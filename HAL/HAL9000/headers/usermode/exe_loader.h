#pragma once

// A light wrapper which abstracts away executable formats

typedef enum _EXE_FORMAT
{
    ExecutableFormatELF,
    ExecutableFormatPE,
    ExecutableFormatUnknown
} EXE_FORMAT;

// Opaque pointer to our internal context structure
typedef PVOID EXE_LOADER_CONTEXT;

// Allocate context
STATUS
ExecutableLoaderPreinit(
    EXE_LOADER_CONTEXT *Context
    );

// Parse and validate executable header
STATUS
ExecutableLoaderInit(
    EXE_LOADER_CONTEXT Context,
    PVOID Image,
    DWORD ImageSize
    );

// Initialize context from PE header
STATUS
ExectuableLoaderInitFromPEHeader(
    EXE_LOADER_CONTEXT *Context,
    PPE_NT_HEADER_INFO HeaderInfo
    );

// Initialize context from Elf header
STATUS
ExectuableLoaderInitFromElfHeader(
    EXE_LOADER_CONTEXT *Context,
    PVOID PhysicalImageBase,
    PVOID VirtualImageBase,
    DWORD ImageSize,
    PVOID HeaderInfo
    );

// Get executable format
STATUS
ExecutableLoaderGetFormat(
    EXE_LOADER_CONTEXT Context,
    EXE_FORMAT *Format
    );

// Get physical ImageBase (where is the executable loaded in memory)
STATUS
ExecutableLoaderGetPhysicalImageBase(
    EXE_LOADER_CONTEXT Context,
    PVOID *ImageBase
    );

// Get virtual ImageBase (VA of preferred ImageBase for PE)
STATUS
ExecutableLoaderGetVirtualImageBase(
    EXE_LOADER_CONTEXT Context,
    PVOID *ImageBase
    );

// Get size of image in memory
STATUS
ExecutableLoaderGetImageSize(
    EXE_LOADER_CONTEXT Context,
    DWORD *Size 
    );

// Get executable entry virtual address
STATUS
ExecutableLoaderGetEntryPoint(
    EXE_LOADER_CONTEXT Context,
    PVOID *EntryPoint
    );

// Memory map executable
STATUS
ExecutableLoaderMemoryMap(
    EXE_LOADER_CONTEXT Context,
    PPAGING_LOCK_DATA PagingData
    );

// Free allocated resources
STATUS
ExecutableLoaderUninit(
    EXE_LOADER_CONTEXT *Context
    );
