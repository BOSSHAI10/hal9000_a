#include "HAL9000.h"
#include "pmm.h"
#include "bootinfo.h"
#include "bitmap.h"
#include "synch.h"

typedef struct _MEMORY_REGION_LIST
{
    EFI_MEMORY_TYPE     Type;
    DWORD               NumberOfEntries;
} MEMORY_REGION_LIST, *PMEMORY_REGION_LIST;

typedef struct _PMM_DATA
{
    // Both of the highest physical address values are setup on initialization and
    // remain unchanged, i.e. if the highest available address becomes later reserved
    // HighestPhysicalAddressAvailable will not change.

    // This points to the end of the highest physical address available in the system
    // i.e. this is not reserved and can be used
    // E.g: if the last available memory region starts at 0xFFFE'0000 and occupies a
    // page HighestPhysicalAddressAvailable will be 0xFFFE'1000
    PHYSICAL_ADDRESS    HighestPhysicalAddressAvailable;

    // This includes the memory already reserved in the system, it is greater than or
    // equal to HighestPhysicalAddressAvailable, depending on the arrangement of
    // memory in the system.
    PHYSICAL_ADDRESS    HighestPhysicalAddressPresent;

    // Total size of available memory over 1MB
    QWORD               PhysicalMemorySize;

    MEMORY_REGION_LIST  MemoryRegionList[EfiMaxMemoryType];

    LOCK                AllocationLock;

    _Guarded_by_(AllocationLock)
    BITMAP              AllocationBitmap;
} PMM_DATA, *PPMM_DATA;

static PMM_DATA m_pmmData;

static
BOOLEAN
_PmmIsMemoryUsable(
    EFI_MEMORY_TYPE MemoryType
    );

static
void
_PmmDetermineMemoryLimits(
    IN                                  HAL_MEMORY_MAP*             MemoryMap,
    OUT                                 QWORD*                      AvailableSystemMemory,
    OUT                                 PHYSICAL_ADDRESS*           HighestPresentPhysicalAddress,
    OUT                                 PHYSICAL_ADDRESS*           HighestAvailablePhysicalAddress,
    INOUT_UPDATES_ALL(EfiMaxMemoryType) PMEMORY_REGION_LIST         MemoryRegions
    );

static
void
_PmmInitializeAllocationBitmap(
    IN                          PVOID                       CurrentVirtualAddress,
    IN                          QWORD                       HighestMemoryAddress,
    IN                          HAL_MEMORY_MAP*             MemoryMap,
    OUT                         PBITMAP                     Bitmap,
    OUT                         DWORD*                      SizeReserved
    );

_No_competing_thread_
void
PmmPreinitSystem(
    void
    )
{
    DWORD i;

    memzero(&m_pmmData, sizeof(PMM_DATA));

    for (i = EfiReservedMemoryType; i < EfiMaxMemoryType; ++i)
    {
        m_pmmData.MemoryRegionList[i].Type = i;
    }

    LockInit(&m_pmmData.AllocationLock);
    LockSetName(&m_pmmData.AllocationLock, "PmmAlloc");
}

_No_competing_thread_
STATUS
PmmInitSystem(
    IN          PVOID                   BaseAddress,
    IN          PVOID                   MemoryMap,
    OUT         DWORD*                  SizeReserved
    )
{
    QWORD pagingStructuresSize;
    DWORD sizeReserved;
    const HAL_MEMORY_MAP *mmap = (const HAL_MEMORY_MAP *) MemoryMap;

    if (NULL == BaseAddress)
    {
        return STATUS_INVALID_PARAMETER1;
    }

    if (NULL == mmap)
    {
        return STATUS_INVALID_PARAMETER2;
    }

    if (0 == mmap->Count)
    {
        return STATUS_INVALID_PARAMETER3;
    }

    if (NULL == SizeReserved)
    {
        return STATUS_INVALID_PARAMETER4;
    }

    pagingStructuresSize = 0;
    sizeReserved = 0;

    _PmmDetermineMemoryLimits(MemoryMap,
                              &m_pmmData.PhysicalMemorySize,
                              &m_pmmData.HighestPhysicalAddressPresent,
                              &m_pmmData.HighestPhysicalAddressAvailable,
                              m_pmmData.MemoryRegionList
                              );

    LOG("Physical memory size: 0x%X\n", m_pmmData.PhysicalMemorySize );
    LOG("Highest Physical address present: 0x%X\n", m_pmmData.HighestPhysicalAddressPresent);
    LOG("Highest Physical address available: 0x%X\n", m_pmmData.HighestPhysicalAddressAvailable);

    _PmmInitializeAllocationBitmap(BaseAddress,
                                   (QWORD) m_pmmData.HighestPhysicalAddressPresent,
                                   MemoryMap,
                                   &m_pmmData.AllocationBitmap,
                                   &sizeReserved
                                   );

    LOG("_PmmInitializeAllocationBitmap completed successfully\n");

    *SizeReserved = AlignAddressUpper( sizeReserved, PAGE_SIZE );

    return STATUS_SUCCESS;
}

PTR_SUCCESS
PHYSICAL_ADDRESS
PmmReserveMemoryEx(
    IN          DWORD                   NoOfFrames,
    IN_OPT      PHYSICAL_ADDRESS        MinPhysAddr
    )
{
    DWORD idx;
    QWORD startIdx;

    INTR_STATE oldState;

    if( 0 == NoOfFrames )
    {
        return NULL;
    }

    if (!IsAddressAligned(MinPhysAddr, PAGE_SIZE))
    {
        return NULL;
    }

    startIdx = (QWORD) MinPhysAddr / PAGE_SIZE;
    if (startIdx > MAX_DWORD)
    {
        return NULL;
    }

    LockAcquire( &m_pmmData.AllocationLock, &oldState);
    idx = BitmapScanFromAndFlip(&m_pmmData.AllocationBitmap, (DWORD) startIdx, NoOfFrames, FALSE );
    if (MAX_DWORD == idx)
    {
        LockRelease( &m_pmmData.AllocationLock, oldState);
        return NULL;
    }

    LockRelease( &m_pmmData.AllocationLock, oldState);

    return (PHYSICAL_ADDRESS) ( (QWORD) idx * PAGE_SIZE );
}

void
PmmReleaseMemory(
    IN          PHYSICAL_ADDRESS        PhysicalAddr,
    IN          DWORD                   NoOfFrames
    )
{
    QWORD index;
    INTR_STATE oldState;

    ASSERT( IsAddressAligned(PhysicalAddr, PAGE_SIZE));

    index = (QWORD) PhysicalAddr / PAGE_SIZE;

    ASSERT( index <= MAX_DWORD);

    LockAcquire( &m_pmmData.AllocationLock, &oldState);
    BitmapClearBits(&m_pmmData.AllocationBitmap, (DWORD) index, NoOfFrames);
    LockRelease( &m_pmmData.AllocationLock, oldState);
}

QWORD
PmmGetTotalSystemMemory(
    void
    )
{
    return m_pmmData.PhysicalMemorySize;
}

PHYSICAL_ADDRESS
PmmGetHighestPhysicalMemoryAddressPresent(
    void
    )
{
    return m_pmmData.HighestPhysicalAddressPresent;
}

PHYSICAL_ADDRESS
PmmGetHighestPhysicalMemoryAddressAvailable(
    void
    )
{
    return m_pmmData.HighestPhysicalAddressAvailable;
}

static
void
_PmmDetermineMemoryLimits(
    IN                                  HAL_MEMORY_MAP*             MemoryMap,
    OUT                                 QWORD*                      AvailableSystemMemory,
    OUT                                 PHYSICAL_ADDRESS*           HighestPresentPhysicalAddress,
    OUT                                 PHYSICAL_ADDRESS*           HighestAvailablePhysicalAddress,
    INOUT_UPDATES_ALL(EfiMaxMemoryType) PMEMORY_REGION_LIST         MemoryRegions
    )
{
    DWORD i;
    QWORD sizeOfAvailableMemory;
    QWORD highestMemoryAddressPresent;
    QWORD highestMemoryAddressAvailable;
    const BYTE *mmap;
    DWORD memoryType;
    QWORD length;

    ASSERT(NULL != MemoryMap);
    ASSERT(0 != MemoryMap->Count);
    ASSERT(NULL != AvailableSystemMemory);
    ASSERT(NULL != HighestPresentPhysicalAddress);
    ASSERT(NULL != HighestAvailablePhysicalAddress);
    ASSERT(NULL != MemoryRegions);

    sizeOfAvailableMemory = 0;
    highestMemoryAddressPresent = 0;
    highestMemoryAddressAvailable = 0;
    
    mmap = (const BYTE*) MemoryMap->MapAddress;

    // LOG("We have %d entries at %X\n", MemoryMap->Count, MemoryMap->MapAddress);
    // LOG("Descriptor size is %d\n", MemoryMap->DescriptorSize);
    for (i = 0; i < MemoryMap->Count; ++i)
    {
        EFI_MEMORY_DESCRIPTOR *memoryDescriptor = (EFI_MEMORY_DESCRIPTOR *) mmap;
        mmap += MemoryMap->DescriptorSize;
        // LOG("\n");
        // LOG("Entry %d at %X\n", i, mmap);
        // LOG("Type %d\n", memoryDescriptor->Type);
        // LOG("PhysicalStart %X\n", memoryDescriptor->PhysicalStart);
        // LOG("NumberOfpages %X\n", memoryDescriptor->NumberOfPages);
        // LOG("\n");
        memoryType = memoryDescriptor->Type;
        length = memoryDescriptor->NumberOfPages * PAGE_SIZE;

        if (memoryDescriptor->Type == EfiReservedMemoryType)
        {
            continue;
        }

        if (memoryDescriptor->PhysicalStart + length > highestMemoryAddressPresent)
        {
            highestMemoryAddressPresent = memoryDescriptor->PhysicalStart + length;
        }

        MemoryRegions[memoryType].NumberOfEntries++;

        if (!_PmmIsMemoryUsable(memoryType))
        {
            // we only care about usable RAM memory
            continue;
        }

        if (memoryDescriptor->PhysicalStart + length - 1 > highestMemoryAddressAvailable)
        {
            highestMemoryAddressAvailable = memoryDescriptor->PhysicalStart + length;
        }

        sizeOfAvailableMemory = sizeOfAvailableMemory + length; 
    }

    *AvailableSystemMemory = sizeOfAvailableMemory;
    *HighestPresentPhysicalAddress = (PHYSICAL_ADDRESS) highestMemoryAddressPresent;
    *HighestAvailablePhysicalAddress = (PHYSICAL_ADDRESS) highestMemoryAddressAvailable;
}

static
void
_PmmInitializeAllocationBitmap(
    IN                          PVOID                       CurrentVirtualAddress,
    IN                          QWORD                       HighestMemoryAddress,
    IN                          HAL_MEMORY_MAP*             MemoryMap,
    OUT                         PBITMAP                     Bitmap,
    OUT                         DWORD*                      SizeReserved
    )
{
    DWORD bitmapSize;
    QWORD noOfPhysicalFrames;
    DWORD i;
    const BYTE* mmap;
    DWORD memoryType;

    LOG_FUNC_START;

    ASSERT(NULL != CurrentVirtualAddress);
    ASSERT( 0 != HighestMemoryAddress );
    ASSERT( NULL != Bitmap );
    ASSERT( NULL != SizeReserved );

    noOfPhysicalFrames = HighestMemoryAddress / PAGE_SIZE;
    ASSERT( noOfPhysicalFrames <= MAX_DWORD);

    bitmapSize = BitmapPreinit(Bitmap, (DWORD) noOfPhysicalFrames);
    BitmapInit(Bitmap, CurrentVirtualAddress );

    LOG("Bitmap size: %u B\n", bitmapSize );

    *SizeReserved = bitmapSize;

    // The idea here is to reserve all possible physical memory
    // PA 0 ----> HighestMemoryAddress
    // and then mark as free only only usable RAM memory over 1MB
    // This means in-existent and reserved system memory will never be used

    PmmReserveMemory(BitmapGetMaxElementCount(Bitmap));

    LOG("All memory is now reserved\n");

    mmap = (const BYTE*) MemoryMap->MapAddress;

    for (i = 0; i < MemoryMap->Count; ++i)
    {
        EFI_MEMORY_DESCRIPTOR *memoryDescriptor = (EFI_MEMORY_DESCRIPTOR *) mmap;
        mmap += MemoryMap->DescriptorSize;

        memoryType =  memoryDescriptor->Type;

        if (!_PmmIsMemoryUsable(memoryType))
        {
            // we only care about usable RAM memory
            continue;
        }

        if (memoryDescriptor->PhysicalStart < 1 * MB_SIZE)
        {
            // we won't be allocating any memory under 1MB
            continue;
        }

        DWORD noOfFrames = (DWORD) memoryDescriptor->NumberOfPages;
        PHYSICAL_ADDRESS physAddr = (PHYSICAL_ADDRESS) AlignAddressUpper(memoryDescriptor->PhysicalStart, PAGE_SIZE);

        ASSERT( noOfFrames <= MAX_DWORD);

        // here it is necessary to use PmmReleaseMemory
        PmmReleaseMemory(physAddr, noOfFrames );

        LOG("Releasing %d frames of memory starting from PA 0x%X\n", noOfFrames, physAddr );
    }

    LOG_FUNC_END;
}

static
BOOLEAN
_PmmIsMemoryUsable(
    EFI_MEMORY_TYPE MemoryType
    )
{
    return (
        // Bootloader allocates few pages as EfiLoaderData, including boot module data
        // so we consider it not usable
        MemoryType == EfiLoaderCode ||
        MemoryType == EfiBootServicesCode ||
        MemoryType == EfiBootServicesData ||
        MemoryType == EfiConventionalMemory ||
        MemoryType == EfiPersistentMemory // standard says it can be used as EfiConventionalMemory
    );
}
