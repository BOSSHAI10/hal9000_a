#pragma once

#include <Uefi.h>

#define PAGE_SIZE 4096

UINT8
MemoryEquals(
    const void *A,
    const void *B,
    UINT64 Size
    );

void 
SetMemory(
    void *Address, 
    const char Value, 
    UINT64 Count
    ); 

void
CopyMemory(
    void *Destination,
    void *Source,
    UINT64 Count
    );

void
CopyWcharAsChar(
    char *Destination,
    CHAR16 *Source,
    UINT64 Count
    );

void* 
AllocateZeroedPages(
    EFI_SYSTEM_TABLE *ST,
    EFI_MEMORY_TYPE MemoryType,
    UINTN NumberOfPages
    );

void*
AllocateZeroedPagesAtAddress(
    EFI_SYSTEM_TABLE *ST,
    EFI_MEMORY_TYPE MemoryType,
    EFI_PHYSICAL_ADDRESS Address,
    UINTN NumberOfPages
    );

void*
AllocateZeroedPagesMaxAddress(
    EFI_SYSTEM_TABLE *ST,
    EFI_MEMORY_TYPE MemoryType,
    EFI_PHYSICAL_ADDRESS Address,
    UINTN NumberOfPages
    );

EFI_STATUS
FreePages(
    EFI_SYSTEM_TABLE *ST,
    UINT64 Address,
    UINT64 PageCount
);

UINT64
GetPageCount(
    UINT64 Size
    );

void* 
AllocateFromPool(
    EFI_SYSTEM_TABLE *ST,
    UINTN Size
    );

UINT8 
FreeFromPool(
    EFI_SYSTEM_TABLE *ST,
    void *Buffer
    );

void*
ReallocateFromPool(
    EFI_SYSTEM_TABLE *ST, 
    UINTN Size, 
    UINTN NewSize, 
    void *Buffer
    );
