#pragma once

#include <Uefi.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/SimpleFileSystem.h>
#include <Guid/FileInfo.h>

EFI_FILE_HANDLE
GetRootDirectory(
    EFI_SYSTEM_TABLE *ST,
    EFI_HANDLE Image
    ); 

UINT64 
GetFileSize(
    EFI_SYSTEM_TABLE *ST, 
    EFI_FILE_HANDLE FileHandle
    );

EFI_STATUS
ListDirectory(
    EFI_SYSTEM_TABLE *ST,
    EFI_FILE_HANDLE RootDirectory,
    CHAR16 *Path,
    EFI_FILE_INFO ***Contents,
    UINT64 *Count
    );

EFI_STATUS
OpenFileForRead(
    EFI_FILE_HANDLE RootDirectory, 
    CHAR16 *Path,
    EFI_FILE_HANDLE *FileHandle
    ); 

EFI_STATUS
CloseFileHandle(
    EFI_FILE_HANDLE FileHandle
    );

UINT64
LoadFileToMemory(
    EFI_SYSTEM_TABLE *ST,
    EFI_FILE_HANDLE FileHandle,
    UINT64 Offset,
    UINT64 *PageCount
    );

UINT64
LoadFileToMemoryAt(
    EFI_SYSTEM_TABLE *ST,
    EFI_FILE_HANDLE FileHandle,
    UINT64 Offset,
    UINT64 Address,
    UINT64 *PageCount
    ); 
