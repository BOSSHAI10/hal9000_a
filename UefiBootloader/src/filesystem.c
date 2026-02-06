#include <Bootloader/filesystem.h>

#include <Bootloader/console.h>
#include <Bootloader/memory.h>

EFI_FILE_HANDLE
GetRootDirectory(
    EFI_SYSTEM_TABLE *ST,
    EFI_HANDLE Image
    ) 
{
    EFI_LOADED_IMAGE *loadedImage = NULL;
    EFI_GUID lipGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *ioVolume;
    EFI_GUID fsGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    EFI_FILE_HANDLE volume;
    EFI_STATUS status = ST->BootServices->HandleProtocol(Image, &lipGuid, (VOID **) &loadedImage); 
    if (status != EFI_SUCCESS) 
    {
        PrintString(ST, EFI_RED, L"Could not obtain EFI_LOADED_IMAGE_PROTOCOL");
        return NULL;
    }
    status = ST->BootServices->HandleProtocol(loadedImage->DeviceHandle, &fsGuid, (VOID **) &ioVolume);
    if (status != EFI_SUCCESS) 
    {
        PrintString(ST, EFI_RED, L"Could not obtain EFI_FILE_SYSTEM_PROTOCOL");
        return NULL;
    }
    status = ioVolume->OpenVolume(ioVolume, &volume);
    if (status != EFI_SUCCESS) 
    {
        return NULL;
    }
    return volume;
}

UINT64 
GetFileSize(
    EFI_SYSTEM_TABLE *ST, 
    EFI_FILE_HANDLE FileHandle
    )
{
    void *buffer = AllocateFromPool(ST, 256);
    UINTN bufferSize = 256;
    EFI_GUID fileInfoGuid = EFI_FILE_INFO_ID;
    while (EFI_ERROR(FileHandle->GetInfo(FileHandle, &fileInfoGuid, &bufferSize, buffer)))
    {
        FreeFromPool(ST, buffer);
        buffer = AllocateFromPool(ST, bufferSize);
    }
    UINT64 fileSize = ((EFI_FILE_INFO *) buffer)->FileSize;
    FreeFromPool(ST, buffer);
    return fileSize;
}

// we return first 512 entries
EFI_STATUS
ListDirectory(
    EFI_SYSTEM_TABLE *ST,
    EFI_FILE_HANDLE RootDirectory,
    CHAR16 *Path,
    EFI_FILE_INFO ***Contents,
    UINT64 *Count
    )
{
    void *buffer = AllocateFromPool(ST, 512);
    UINTN bufferSize = 512;
    EFI_GUID fileInfoGuid = EFI_FILE_INFO_ID;
    EFI_FILE_HANDLE directoryHandle;
    EFI_STATUS status;
    if (EFI_ERROR((status = OpenFileForRead(RootDirectory, Path, &directoryHandle))))
        return status;

    while (EFI_ERROR(directoryHandle->GetInfo(directoryHandle, &fileInfoGuid, &bufferSize, buffer)))
    {
        FreeFromPool(ST, buffer);
        buffer = AllocateFromPool(ST, bufferSize);
    }
    UINT64 fileAttribute = ((EFI_FILE_INFO *) buffer)->Attribute;
    FreeFromPool(ST, buffer);
    if (!(fileAttribute & EFI_FILE_DIRECTORY))
    {
        *Contents = NULL;
        *Count = 0;
        CloseFileHandle(directoryHandle);
        return EFI_ACCESS_DENIED;
    }

    *Contents = AllocateFromPool(ST, 512 * sizeof(void *));
    EFI_FILE_INFO **ret = *Contents;

    UINT32 i = 0;
    bufferSize = 512;
    buffer = AllocateFromPool(ST, 512);
    status = directoryHandle->Read(directoryHandle, &bufferSize, buffer);
    while (i < 512 && !EFI_ERROR(status))
    {
        if (!bufferSize)
            break;
        ret[i] = buffer;
        bufferSize = 512;
        buffer = AllocateFromPool(ST, 512);
        status = directoryHandle->Read(directoryHandle, &bufferSize, buffer);
        i++; 
    }

    FreeFromPool(ST, buffer);
    CloseFileHandle(directoryHandle);
    *Count = i;

    return EFI_SUCCESS;
}

EFI_STATUS
OpenFileForRead(
    EFI_FILE_HANDLE RootDirectory, 
    CHAR16 *Path,
    EFI_FILE_HANDLE *FileHandle
    ) 
{
    return RootDirectory->Open(RootDirectory, FileHandle, 
        Path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
} 

EFI_STATUS
CloseFileHandle(
    EFI_FILE_HANDLE FileHandle
    )
{
    return FileHandle->Close(FileHandle);
}

UINT64
LoadFileToMemory(
    EFI_SYSTEM_TABLE *ST,
    EFI_FILE_HANDLE FileHandle,
    UINT64 Offset,
    UINT64 *PageCount
    ) 
{
    if (EFI_ERROR(FileHandle->SetPosition(FileHandle, Offset)))
        return 0;
    UINT64 fileSize = GetFileSize(ST, FileHandle);
    *PageCount = GetPageCount(fileSize);
    void *buffer = AllocateZeroedPagesMaxAddress(ST, EfiLoaderData, (EFI_PHYSICAL_ADDRESS) (BASE_4GB - 1) , *PageCount);
    if (!buffer)
        return 0;
    UINT64 bufferSize = *PageCount * PAGE_SIZE;
    EFI_STATUS status = FileHandle->Read(FileHandle, &bufferSize, buffer);
    if (EFI_ERROR(status))
    {
        ST->BootServices->FreePages((EFI_PHYSICAL_ADDRESS) buffer, *PageCount);
        return 0;
    }
    return (UINT64) buffer;
}

// We can allocate pages at a specfic address but OVMF marks 1MB reserved so we just force it
// and hope it works for now, memory dump from QEMU says it is all 0xFF
UINT64
LoadFileToMemoryAt(
    EFI_SYSTEM_TABLE *ST,
    EFI_FILE_HANDLE FileHandle,
    UINT64 Offset,
    UINT64 Address,
    UINT64 *PageCount
    ) 
{
    if (EFI_ERROR(FileHandle->SetPosition(FileHandle, Offset)))
        return 0;
    UINT64 fileSize = GetFileSize(ST, FileHandle);
    *PageCount = GetPageCount(fileSize);
    // void *buffer = AllocateZeroedPagesAtAddress(ST, EfiLoaderData, Address, *PageCount);
    void *buffer = (void *) Address;
    UINT64 bufferSize = *PageCount * PAGE_SIZE;
    EFI_STATUS status = FileHandle->Read(FileHandle, &bufferSize, buffer);
    if (EFI_ERROR(status))
    {
        ST->BootServices->FreePages((EFI_PHYSICAL_ADDRESS) buffer, *PageCount);
        return 0;
    }
    return (UINT64) buffer;
}
