#include <Bootloader/memory.h>

// dumb version
UINT8
MemoryEquals(
    const void *A,
    const void *B,
    UINT64 Size
    )
{
    const char *p1 = (const char *) A;
    const char *p2 = (const char *) B;
    for (int i = 0; i < Size; i++)
        if (p1[i] != p2[i])
            return 0;
    return 1;
}

// dumb version
void 
SetMemory(
    void *Address, 
    const char Value, 
    UINT64 Count
    ) 
{
    char *buff = (char *) Address;
    for (UINT64 i = 0; i < Count; i++)
        buff[i] = Value;
} 

// dumb version
void
CopyMemory(
    void *Destination,
    void *Source,
    UINT64 Count
    )
{
    char *source = (char *) Source;
    char *destination = (char *) Destination;
    for (UINT64 i = 0; i < Count; i++)
        destination[i] = source[i];
}

void
CopyWcharAsChar(
    char *Destination,
    CHAR16 *Source,
    UINT64 Count
    )
{
    for (UINT64 i = 0; i < Count; i++)
        Destination[i] = (char) Source[i];
}

void* 
AllocateZeroedPages(
    EFI_SYSTEM_TABLE *ST,
    EFI_MEMORY_TYPE MemoryType,
    UINTN NumberOfPages
    ) 
{
    EFI_PHYSICAL_ADDRESS ret;
    EFI_STATUS status = ST->BootServices->AllocatePages(AllocateAnyPages, MemoryType, NumberOfPages, &ret);
    if (EFI_ERROR(status))
        return NULL;
    SetMemory((void *) ret, 0, NumberOfPages * PAGE_SIZE);
    return (void *) ret;
}

void*
AllocateZeroedPagesAtAddress(
    EFI_SYSTEM_TABLE *ST,
    EFI_MEMORY_TYPE MemoryType,
    EFI_PHYSICAL_ADDRESS Address,
    UINTN NumberOfPages
    )
{
    EFI_PHYSICAL_ADDRESS ret = Address;
    EFI_STATUS status = ST->BootServices->AllocatePages(AllocateAddress, MemoryType, NumberOfPages, &ret);
    if (EFI_ERROR(status))
        return NULL;
    SetMemory((void *) ret, 0, NumberOfPages * PAGE_SIZE);
    return (void *) ret;
}

void*
AllocateZeroedPagesMaxAddress(
    EFI_SYSTEM_TABLE *ST,
    EFI_MEMORY_TYPE MemoryType,
    EFI_PHYSICAL_ADDRESS Address,
    UINTN NumberOfPages
    )
{
    EFI_PHYSICAL_ADDRESS ret = Address;
    EFI_STATUS status = ST->BootServices->AllocatePages(AllocateMaxAddress, MemoryType, NumberOfPages, &ret);
    if (EFI_ERROR(status))
        return NULL;
    SetMemory((void *) ret, 0, NumberOfPages * PAGE_SIZE);
    return (void *) ret;
}

EFI_STATUS
FreePages(
    EFI_SYSTEM_TABLE *ST,
    UINT64 Address,
    UINT64 PageCount
)
{
    return ST->BootServices->FreePages((EFI_PHYSICAL_ADDRESS) Address, PageCount);
}

UINT64
GetPageCount(
    UINT64 Size
    )
{
    return Size / PAGE_SIZE + (Size % PAGE_SIZE != 0);
}

void* 
AllocateFromPool(
    EFI_SYSTEM_TABLE *ST,
    UINTN Size
    )
{
    void *buffer;
    if (EFI_ERROR(ST->BootServices->AllocatePool(EfiLoaderData, Size, (void **) &buffer)))
        return (void *) NULL;
    return buffer;
}

UINT8 
FreeFromPool(
    EFI_SYSTEM_TABLE *ST,
    void *Buffer
    )
{
    if (EFI_ERROR(ST->BootServices->FreePool(Buffer)))
        return 0;
    return 1;
}

void*
ReallocateFromPool(
    EFI_SYSTEM_TABLE *ST, 
    UINTN Size, 
    UINTN NewSize, 
    void *Buffer
    )
{
    void *newBuffer = AllocateFromPool(ST, NewSize);
    if (!newBuffer)
        return (void *) NULL;
    CopyMemory(newBuffer, Buffer, Size);
    FreeFromPool(ST, Buffer);
    return newBuffer;
}
