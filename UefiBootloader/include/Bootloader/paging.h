#pragma once

#include <Uefi.h>

void* PagingInit(
    EFI_SYSTEM_TABLE *ST
    );

EFI_STATUS
MemoryMapPages(
    EFI_SYSTEM_TABLE *ST,
    void *Pml4Address,
    void *PhysicalAddress,
    void *VirtualAddress,
    UINT64 NumberOfPages
    );

EFI_STATUS 
MemoryMapPages1GB(
    EFI_SYSTEM_TABLE *ST,
    void *Pml4Address,
    void *PhysicalAddress,
    void *VirtualAddress,
    UINT64 SizeinGb
    );
