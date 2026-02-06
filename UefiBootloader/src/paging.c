#include <Bootloader/paging.h>

#include <Bootloader/memory.h>

#define PML4_INDEX_MASK  0x00000FF8000000000 
#define PML4_INDEX_SHIFT 39
#define PDP_INDEX_MASK   0x00000007FC0000000
#define PDP_INDEX_SHIFT  30
#define PD_INDEX_MASK    0x0000000003FE00000
#define PD_INDEX_SHIFT   21
#define PT_INDEX_MASK    0x000000000001FF000
#define PT_INDEX_SHIFT   12
#define PAGE_OFFSET_MASK 0x00000000000000FFF

void* PagingInit(
    EFI_SYSTEM_TABLE *ST
    ) 
{
    void *Pml4 = AllocateZeroedPagesMaxAddress(ST,
                                               EfiLoaderData,
                                               (EFI_PHYSICAL_ADDRESS) (BASE_4GB - 1),
                                               1);

    if(Pml4 == NULL)
        return NULL;

    return Pml4;
}

static
EFI_STATUS 
_MemoryMapPage(
    EFI_SYSTEM_TABLE *ST,
    void *Pml4Address,
    void *PhysicalAddress,
    void *VirtualAddress
    ) 
{
    UINT64 Index4 = ((UINT64) VirtualAddress & PML4_INDEX_MASK) >> PML4_INDEX_SHIFT;
    UINT64 *Pml4Table = (UINT64 *) Pml4Address;

    UINT64 *PdpTable;
    if(!Pml4Table[Index4]) {
        PdpTable = AllocateZeroedPagesMaxAddress(ST,
                                                 EfiLoaderData,
                                                 (EFI_PHYSICAL_ADDRESS) (BASE_4GB - 1),
                                                 1);
        if(!PdpTable)
            return EFI_OUT_OF_RESOURCES;
        Pml4Table[Index4] = (UINT64) PdpTable | 0x3;
    } else
        PdpTable = (UINT64 *) (Pml4Table[Index4] & (~0x3));

    UINT64 Index3 = ((UINT64) VirtualAddress & PDP_INDEX_MASK) >> PDP_INDEX_SHIFT;
    UINT64 *PdTable;
    if(!PdpTable[Index3]) {
        PdTable = AllocateZeroedPagesMaxAddress(ST,
                                                EfiLoaderData,
                                                (EFI_PHYSICAL_ADDRESS) (BASE_4GB - 1),
                                                1);
        if(!PdTable)
            return EFI_OUT_OF_RESOURCES;
        PdpTable[Index3] = (UINT64) PdTable | 0x3;
    } else
        PdTable = (UINT64 *) (PdpTable[Index3] & (~0x3));

    UINT64 Index2 = ((UINT64) VirtualAddress & PD_INDEX_MASK) >> PD_INDEX_SHIFT;
    UINT64 *PtTable;
    if(!PdTable[Index2]) {
        PtTable = AllocateZeroedPagesMaxAddress(ST,
                                                EfiLoaderData,
                                                (EFI_PHYSICAL_ADDRESS) (BASE_4GB - 1),
                                                1);
        if(!PtTable)
            return EFI_OUT_OF_RESOURCES;
        PdTable[Index2] = (UINT64) PtTable | 0x3;
    } else
        PtTable = (UINT64 *) (PdTable[Index2] & (~0x3));
    
    UINT64 Index1 = ((UINT64) VirtualAddress & PT_INDEX_MASK) >> PT_INDEX_SHIFT;
    PtTable[Index1] = (UINT64) PhysicalAddress | 0x3;
    
    return EFI_SUCCESS;
}

EFI_STATUS
MemoryMapPages(
    EFI_SYSTEM_TABLE *ST,
    void *Pml4Address,
    void *PhysicalAddress,
    void *VirtualAddress,
    UINT64 NumberOfPages
    )
{
    UINT64 pAddr = (UINT64) PhysicalAddress;
    UINT64 vAddr = (UINT64) VirtualAddress;
    for (UINT64 i = 0;i < NumberOfPages;i++) {
        if (_MemoryMapPage(ST, Pml4Address, (void *) pAddr, (void *) vAddr) != EFI_SUCCESS)
            return EFI_OUT_OF_RESOURCES;
        pAddr += PAGE_SIZE;
        vAddr += PAGE_SIZE;
    }

    return EFI_SUCCESS;
}

static
EFI_STATUS 
_MemoryMap1GB(
    EFI_SYSTEM_TABLE *ST,
    void *Pml4Address,
    void *PhysicalAddress,
    void *VirtualAddress
    ) 
{
    UINT64 Index4 = ((UINT64) VirtualAddress & PML4_INDEX_MASK) >> PML4_INDEX_SHIFT;
    UINT64 *Pml4Table = (UINT64 *) Pml4Address;

    UINT64 *PdpTable;
    if(!Pml4Table[Index4]) {
        PdpTable = AllocateZeroedPagesMaxAddress(ST,
                                                 EfiLoaderData,
                                                 (EFI_PHYSICAL_ADDRESS) (BASE_4GB - 1),
                                                 1);
        if(!PdpTable)
            return EFI_OUT_OF_RESOURCES;
        Pml4Table[Index4] = (UINT64) PdpTable | 0x3;
    } else
        PdpTable = (UINT64 *) (Pml4Table[Index4] & (~0x3));

    UINT64 Index2 = ((UINT64) VirtualAddress & PD_INDEX_MASK) >> PD_INDEX_SHIFT;
    PdpTable[Index2] = (UINT64) PhysicalAddress | (1 << 7) | 0x3;

    return EFI_SUCCESS;
}

EFI_STATUS 
MemoryMapPages1GB(
    EFI_SYSTEM_TABLE *ST,
    void *Pml4Address,
    void *PhysicalAddress,
    void *VirtualAddress,
    UINT64 SizeInGb
    )
{
    UINT64 pAddr = (UINT64) PhysicalAddress;
    UINT64 vAddr = (UINT64) VirtualAddress;
    for (UINT64 i = 0;i < SizeInGb;i++) {
        if (_MemoryMap1GB(ST, Pml4Address, (void *) pAddr, (void *) vAddr) != EFI_SUCCESS)
            return EFI_OUT_OF_RESOURCES;
        pAddr += BASE_1GB;
        vAddr += BASE_1GB;
    }

    return EFI_SUCCESS;
}
