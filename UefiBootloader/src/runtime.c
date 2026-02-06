#include <Bootloader/runtime.h>

#include <Bootloader/memory.h>

#define UEFI_RUNTIME_SERVICES_VIRTUAL_ADDRESS 0xFFFF8A0000000000

// We will map the UEFI runtime starting from the address above continously
// until we are done
// See UEFI spec for details about how to do this
EFI_STATUS
SwitchToVirtualAdressingMode(
    EFI_SYSTEM_TABLE *ST,
    HAL_MEMORY_MAP *MemoryMap,
    UINT32 DescriptorVersion,
    UINT64 *UefiRuntimeAddress,
    UINT64 *UefiRuntimeDescriptorCount
    )
{
    EFI_STATUS status;
    UINT64 currentVirtualAddress = UEFI_RUNTIME_SERVICES_VIRTUAL_ADDRESS;
    UINT64 count = 0;
    UINT8 *buff = (UINT8 *) MemoryMap->MapAddress;
    
    *UefiRuntimeAddress = *UefiRuntimeDescriptorCount = 0;

    for (UINT32 i = 0; i < MemoryMap->Count; i++)
    {
        EFI_MEMORY_DESCRIPTOR *descriptor = (EFI_MEMORY_DESCRIPTOR *) buff;
        buff += MemoryMap->DescriptorSize;
        if (descriptor->Attribute & EFI_MEMORY_RUNTIME)
        {
            descriptor->VirtualStart = (EFI_VIRTUAL_ADDRESS) currentVirtualAddress;
            currentVirtualAddress += descriptor->NumberOfPages * PAGE_SIZE;
            count++;
        }
    }

    // Now let's try to switch to VirtualAdressing
    UINTN MemoryMapSize = MemoryMap->Count * MemoryMap->DescriptorSize;
    status = ST->RuntimeServices->SetVirtualAddressMap(MemoryMapSize,
                                                       MemoryMap->DescriptorSize,
                                                       DescriptorVersion,
                                                       (EFI_MEMORY_DESCRIPTOR *) MemoryMap->MapAddress
                                                       );
    if (status == EFI_SUCCESS)
    {
        *UefiRuntimeAddress = UEFI_RUNTIME_SERVICES_VIRTUAL_ADDRESS;
        *UefiRuntimeDescriptorCount = count;
    }

    return status;
}
