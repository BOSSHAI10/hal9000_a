#include "HAL9000.h"
#include "ex.h"
#include "log.h"
#include "pe_exports.h"
#include "pe_parser.h"
#include "elf_exports.h"
#include "elf_parser.h"
#include "mmu.h"
#include "status.h"
#include "exe_loader.h"

typedef struct
{
    PE_NT_HEADER_INFO PeHeaderInfo;
    Elf64_Ehdr ElfFileHeader;
} _EXE_HEADER;

typedef struct
{
    PVOID Image;
    PVOID VirtualImage;
    DWORD ImageSize;
    EXE_FORMAT Format; 
    _EXE_HEADER Header;
} _EXE_LOADER_CONTEXT;

static
EXE_FORMAT
_ExecutableLoaderDetermineFormat(
    PVOID Image
    );

STATUS
ExecutableLoaderPreinit(
    EXE_LOADER_CONTEXT *Context
    )
{
    if (NULL == Context)
    {
        return STATUS_INVALID_PARAMETER1;
    }

    _EXE_LOADER_CONTEXT *context = ExAllocatePoolWithTag(PoolAllocateZeroMemory,
                                                         sizeof(_EXE_LOADER_CONTEXT),
                                                         HEAP_PROCESS_TAG,
                                                         0);
    if (NULL == context)
    {
        LOG_FUNC_ERROR_ALLOC("ExAllocatePoolWithTag", sizeof(_EXE_LOADER_CONTEXT));
        *Context = NULL;
        return STATUS_HEAP_INSUFFICIENT_RESOURCES;
    }

    *Context = context;

    return STATUS_SUCCESS;
}

STATUS
ExecutableLoaderInit(
    EXE_LOADER_CONTEXT Context,
    PVOID Image,
    DWORD ImageSize
    )
{
    STATUS status;
    EXE_FORMAT format;
    _EXE_LOADER_CONTEXT *context;
    PVOID virtualImage;
    
    status = STATUS_SUCCESS;
    format = ExecutableFormatUnknown;
    context = (_EXE_LOADER_CONTEXT *) Context;
    virtualImage = NULL;

    if (NULL == Context)
    {
        return STATUS_INVALID_PARAMETER1;
    }

    if (NULL == Image)
    {
        return STATUS_INVALID_PARAMETER2;
    }

    if (0 == ImageSize)
    {
        return STATUS_INVALID_PARAMETER3;
    }
    
    format = _ExecutableLoaderDetermineFormat(Image);
    if (format == ExecutableFormatUnknown)
    {
        return STATUS_UNSUPPORTED;
    }

    if (format == ExecutableFormatELF)
    {
        status = ElfRetrieveFileHeader(Image, ImageSize, &(context->Header.ElfFileHeader));
        virtualImage = (PVOID) ((Elf64_Phdr *) PtrOffset(Image, context->Header.ElfFileHeader.e_phoff))->p_vaddr;
    }
    else
    {
        status = PeRetrieveNtHeader(Image, ImageSize, &(context->Header.PeHeaderInfo));
        virtualImage = (PVOID) context->Header.PeHeaderInfo.Preferred.ImageBase; 
    }

    if (!SUCCEEDED(status))
    {
        return status;
    }

    context->Image = Image;
    context->VirtualImage = virtualImage;
    context->ImageSize = ImageSize;
    context->Format = format;
    
    return STATUS_SUCCESS;
}

STATUS
ExectuableLoaderInitFromPEHeader(
    EXE_LOADER_CONTEXT *Context,
    PPE_NT_HEADER_INFO HeaderInfo
    )
{
    STATUS status;
    _EXE_LOADER_CONTEXT *context;

    status = STATUS_SUCCESS;
    context = NULL;

    if (NULL == Context)
    {
        return STATUS_INVALID_PARAMETER1;
    }

    if (NULL == HeaderInfo)
    {
        return STATUS_INVALID_PARAMETER2;
    }

    status = ExecutableLoaderPreinit(Context);
    if (!SUCCEEDED(status))
    {
        return status;
    }

    context = (_EXE_LOADER_CONTEXT *) *Context;
    context->Format = ExecutableFormatPE;
    context->Image = HeaderInfo->ImageBase;
    context->VirtualImage = HeaderInfo->Preferred.ImageBase;
    context->ImageSize = HeaderInfo->Size;
    memcpy(&(context->Header.PeHeaderInfo), HeaderInfo, sizeof(PE_NT_HEADER_INFO));
  
    return STATUS_SUCCESS;
}
STATUS
ExectuableLoaderInitFromElfHeader(
    EXE_LOADER_CONTEXT *Context,
    PVOID PhysicalImageBase,
    PVOID VirtualImageBase,
    DWORD ImageSize,
    PVOID HeaderInfo
    )
{
    STATUS status;
    _EXE_LOADER_CONTEXT *context;

    status = STATUS_SUCCESS;
    context = NULL;

    if (NULL == Context)
    {
        return STATUS_INVALID_PARAMETER1;
    }
 
    if (NULL == PhysicalImageBase)
    {
        return STATUS_INVALID_PARAMETER2;
    }
    
    if (NULL == VirtualImageBase)
    {
        return STATUS_INVALID_PARAMETER3;
    }
    
    if (0 == ImageSize)
    {
        return STATUS_INVALID_PARAMETER4;
    }

    if (NULL == HeaderInfo)
    {
        return STATUS_INVALID_PARAMETER5;
    }

    status = ExecutableLoaderPreinit(Context);
    if (!SUCCEEDED(status))
    {
        return status;
    }

    context = (_EXE_LOADER_CONTEXT *) *Context;
    context->Format = ExecutableFormatELF;
    context->Image = PhysicalImageBase;
    context->VirtualImage = VirtualImageBase;
    context->ImageSize = ImageSize;
    memcpy(&(context->Header.ElfFileHeader), HeaderInfo, sizeof(Elf64_Ehdr));
  
    return STATUS_SUCCESS;
}

STATUS
ExecutableLoaderGetFormat(
    EXE_LOADER_CONTEXT Context,
    EXE_FORMAT *Format
    )
{
    if (NULL == Context)
    {
        return STATUS_INVALID_PARAMETER1;
    }

    if (NULL == Format)
    {
        return STATUS_INVALID_PARAMETER2;
    }

    *Format = ((_EXE_LOADER_CONTEXT *) Context)->Format;

    return STATUS_SUCCESS;
}

STATUS
ExecutableLoaderGetPhysicalImageBase(
    EXE_LOADER_CONTEXT Context,
    PVOID *ImageBase
    )
{
    _EXE_LOADER_CONTEXT *context;

    context = (_EXE_LOADER_CONTEXT *) Context;

    if (NULL == Context)
    {
        return STATUS_INVALID_PARAMETER1;
    }
    
    if (NULL == ImageBase)
    {
        return STATUS_INVALID_PARAMETER2;
    }

    if (context->Format == ExecutableFormatUnknown)
    {
        return STATUS_UNSUPPORTED;
    }

    *ImageBase = context->Image;

    return STATUS_SUCCESS;
}

STATUS
ExecutableLoaderGetVirtualImageBase(
    EXE_LOADER_CONTEXT Context,
    PVOID *ImageBase
    )
{
    _EXE_LOADER_CONTEXT *context;

    context = (_EXE_LOADER_CONTEXT *) Context;

    if (NULL == Context)
    {
        return STATUS_INVALID_PARAMETER1; 
    }

    if (NULL == ImageBase)
    {
        return STATUS_INVALID_PARAMETER2;
    }

    if (context->Format == ExecutableFormatUnknown)
    {
        return STATUS_UNSUPPORTED;
    }

    *ImageBase = context->VirtualImage;

    return STATUS_SUCCESS; 
}

STATUS
ExecutableLoaderGetImageSize(
    EXE_LOADER_CONTEXT Context,
    DWORD *Size 
    )
{
    _EXE_LOADER_CONTEXT *context;

    context = (_EXE_LOADER_CONTEXT *) Context;

    if (NULL == Context)
    {
        return STATUS_INVALID_PARAMETER1; 
    }

    if (NULL == Size)
    {
        return STATUS_INVALID_PARAMETER2;
    }

    if (context->Format == ExecutableFormatUnknown)
    {
        return STATUS_UNSUPPORTED;
    }

    *Size = context->ImageSize;

    return STATUS_SUCCESS; 
}

STATUS
ExecutableLoaderGetEntryPoint(
    EXE_LOADER_CONTEXT Context,
    PVOID *EntryPoint
    )
{
    _EXE_LOADER_CONTEXT *context;
    PVOID entryPoint;

    context = (_EXE_LOADER_CONTEXT *) Context;
    entryPoint = NULL;

    if (NULL == Context)
    {
        return STATUS_INVALID_PARAMETER1; 
    }

    if (NULL == EntryPoint)
    {
        return STATUS_INVALID_PARAMETER2;
    }

    if (context->Format == ExecutableFormatUnknown)
    {
        return STATUS_UNSUPPORTED;
    }

    if (context->Format == ExecutableFormatELF)
    {
        entryPoint = (PVOID) context->Header.ElfFileHeader.e_entry;
    }
    else
    {
        entryPoint = context->Header.PeHeaderInfo.Preferred.AddressOfEntryPoint;
    }

    *EntryPoint = entryPoint;

    return STATUS_SUCCESS; 
}

STATUS
ExecutableLoaderMemoryMap(
    EXE_LOADER_CONTEXT Context,
    PPAGING_LOCK_DATA PagingData
    )
{
    STATUS status;
    _EXE_LOADER_CONTEXT *context;

    status = STATUS_SUCCESS;
    context = (_EXE_LOADER_CONTEXT *) Context;

    if (NULL == Context)
    {
        return STATUS_INVALID_PARAMETER1;
    }

    if (NULL == PagingData)
    {
        return STATUS_INVALID_PARAMETER2;
    }

    if (context->Format == ExecutableFormatUnknown)
    {
        return STATUS_UNSUPPORTED;
    }

    if (context->Format == ExecutableFormatELF)
    {
        status = MmuLoadElf(context->Image,
                            context->ImageSize,
                            &(context->Header.ElfFileHeader),
                            PagingData,
                            0x0
                            );
    }
    else
    {
        status = MmuLoadPe(&(context->Header.PeHeaderInfo), PagingData);
    }

    return status;
}

STATUS
ExecutableLoaderUninit(
    EXE_LOADER_CONTEXT *Context
    )
{
    if (NULL == Context)
    {
        return STATUS_INVALID_PARAMETER1;
    }

    if (NULL == *Context)
    {
        return STATUS_INVALID_PARAMETER1;
    }

    ExFreePoolWithTag(*Context, HEAP_PROCESS_TAG);
    *Context = NULL;

    return STATUS_SUCCESS;
}

static
EXE_FORMAT
_ExecutableLoaderDetermineFormat(
    PVOID Image
    )
{
    WORD *potentialMzSignature = (WORD *) Image;
    DWORD *potentialElfSignature = (DWORD *) Image;

    if (0x5A4D == *potentialMzSignature)
    {
        return ExecutableFormatPE;
    }

    if (0x464C457F == *potentialElfSignature)
    {
        return ExecutableFormatELF;
    }

    return ExecutableFormatUnknown;
}
