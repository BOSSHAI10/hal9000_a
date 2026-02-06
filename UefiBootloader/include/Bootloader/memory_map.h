#pragma once

#include <Uefi.h>

#include <Bootloader/types.h>

UINTN 
GetMemoryMap(
    EFI_SYSTEM_TABLE *ST,
    HAL_MEMORY_MAP *MemoryMap,
    UINT32 *DescriptorVersion
    );
