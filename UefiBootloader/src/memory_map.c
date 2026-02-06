#include <Bootloader/memory_map.h>

#include <Bootloader/memory.h>

UINTN 
GetMemoryMap(
    EFI_SYSTEM_TABLE *ST,
    HAL_MEMORY_MAP *MemoryMap,
    UINT32 *DescriptorVersion
    )
{
    UINTN memoryMapSize = PAGE_SIZE;
    EFI_MEMORY_DESCRIPTOR *memoryMapPointer = (EFI_MEMORY_DESCRIPTOR *) MemoryMap->MapAddress;
    if (!memoryMapPointer)
        memoryMapPointer = AllocateZeroedPagesMaxAddress(ST,
                                                         EfiLoaderData,
                                                         (EFI_PHYSICAL_ADDRESS) (BASE_4GB - 1),
                                                         1);
    UINTN mapKey;
    UINTN descriptorSize;
    UINT32 descriptorVersion;
    UINT32 pageCount = 1;

    EFI_STATUS status;
    while ((status = ST->BootServices->GetMemoryMap(&memoryMapSize, memoryMapPointer, 
            &mapKey, &descriptorSize, &descriptorVersion)) == EFI_BUFFER_TOO_SMALL) 
    {
        FreePages(ST, (UINT64) memoryMapPointer, pageCount);
        memoryMapSize += PAGE_SIZE;
        pageCount++;
        memoryMapPointer = AllocateZeroedPagesMaxAddress(ST,
                                                         EfiLoaderData,
                                                         (EFI_PHYSICAL_ADDRESS) (BASE_4GB - 1),
                                                         pageCount);
    }

    if (status != EFI_SUCCESS || descriptorVersion != EFI_MEMORY_DESCRIPTOR_VERSION)
        return UINT64_MAX;

    MemoryMap->DescriptorSize = (UINT32) descriptorSize;
    MemoryMap->Count = (UINT32) (memoryMapSize / descriptorSize);
    MemoryMap->MapAddress = (UINT64) memoryMapPointer;

    if (DescriptorVersion != NULL)
    {
        *DescriptorVersion = descriptorVersion;
    }
    
    return mapKey;
}
