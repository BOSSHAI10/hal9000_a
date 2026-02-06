#pragma once

#include <Uefi.h>

#include <Bootloader/types.h>

// This funtion makes the UEFI runtime available in virtual memory
// without identity mapping. HAL needs to map this region
// if it wishes to use those services.
EFI_STATUS
SwitchToVirtualAdressingMode(
    EFI_SYSTEM_TABLE *ST,
    HAL_MEMORY_MAP *MemoryMap,
    UINT32 DescriptorVersion,
    UINT64 *UefiRuntimeAddress,
    UINT64 *UefiRuntimeDescriptorCount
    );
