#include "elf_base.h"

#include "elf_exports.h"
#include "elf_parser.h"

STATUS
ElfRetrieveFileHeader(
    PVOID ImageBase,
    DWORD ImageSize,
    Elf64_Ehdr* FileHeader
)
{
    Elf64_Ehdr *fileHdr;

    fileHdr = (Elf64_Ehdr *) ImageBase;

    if (NULL == ImageBase)
    {
        return STATUS_INVALID_PARAMETER1;
    }

    if (0 == ImageSize)
    {
        return STATUS_INVALID_PARAMETER2;
    }

    if (NULL == FileHeader)
    {
        return STATUS_INVALID_PARAMETER3;
    }

    // Check ELF magic
    if (fileHdr->e_ident[EI_MAG0] != ELFMAG0 ||
        fileHdr->e_ident[EI_MAG1] != ELFMAG1 ||
        fileHdr->e_ident[EI_MAG2] != ELFMAG2 ||
        fileHdr->e_ident[EI_MAG3] != ELFMAG3
        )
    {
        return STATUS_UNSUPPORTED;
    }

    // Check ELF class
    if (fileHdr->e_ident[EI_CLASS] != ELFCLASS64)
    {
        return STATUS_IMAGE_NOT_64_BIT;
    }

    // Check endianness
    if (fileHdr->e_ident[EI_DATA] != ELFDATA2LSB)
    {
        return STATUS_UNSUPPORTED;
    }
    
    // Check ELF version
    if (fileHdr->e_ident[EI_VERSION] != EV_CURRENT)
    {
        return STATUS_UNSUPPORTED;
    }

    // Check ABI
    if (fileHdr->e_ident[EI_OSABI] != ELFOSABI_SYSV)
    {
        return STATUS_UNSUPPORTED;
    }

    // Check ELF type
    if (fileHdr->e_type != ET_EXEC)
    {
        return STATUS_UNSUPPORTED;
    }

    // Check machine
    if (fileHdr->e_machine != EM_X86_64)
    {
        return STATUS_IMAGE_NOT_64_BIT;
    }

    // Make sure Program Header immediately follows file header
    // and that file header has the size defined for ELF64
    if (fileHdr->e_ehsize != 0x40 || fileHdr->e_phoff != 0x40)
    {
        return STATUS_UNSUPPORTED;
    }

    // Check that we have program header entries
    // and they are the same size as our struct
    if (fileHdr->e_phnum == 0 ||
        fileHdr->e_phentsize != sizeof(Elf64_Phdr)
        )
    {
        return STATUS_UNSUPPORTED;
    }

    // Check that program header is not out of range
    if (!CHECK_BOUNDS(fileHdr->e_phoff, fileHdr->e_phentsize * fileHdr->e_phnum, 0, ImageSize))
    {
        return STATUS_UNSUPPORTED;
    } 

    memcpy(FileHeader, fileHdr, sizeof(Elf64_Ehdr));

    return STATUS_SUCCESS;
}

STATUS
ElfGetSegment(
    PVOID ImageBase,
    DWORD ImageSize,
    Elf64_Ehdr *FileHeader,
    DWORD SegmentIndex,
    Elf64_Phdr *SegmentInfo
    )
{
    Elf64_Phdr *pEntry;

    pEntry = NULL;

    if (NULL == ImageBase)
    {
        return STATUS_INVALID_PARAMETER1;
    }

    if (0 == ImageSize)
    {
        return STATUS_INVALID_PARAMETER2;
    }

    if (NULL == FileHeader)
    {
        return STATUS_INVALID_PARAMETER3;
    }

    if (SegmentIndex >= FileHeader->e_phnum)
    {
        return STATUS_INVALID_PARAMETER4;
    }

    if (NULL == SegmentInfo)
    {
        return STATUS_INVALID_PARAMETER5;
    }

    pEntry = (Elf64_Phdr *) PtrOffset(ImageBase, FileHeader->e_phoff);
    pEntry = &(pEntry[SegmentIndex]);

    // We cannot load semgents which are supposed to be
    // allocated by the loader, like .bss
    if (pEntry->p_filesz != pEntry->p_memsz)
    {
        return STATUS_INVALID_IMAGE_SIZE;
    }

    // Check if we are in bounds
    if (!CHECK_BOUNDS(pEntry->p_off, pEntry->p_filesz, 0, ImageSize))
    {
        return STATUS_INVALID_IMAGE_SIZE;
    }

    // Make sure we have page alignment
    if (pEntry->p_align != PAGE_SIZE)
    {
        return STATUS_INVALID_IMAGE_SIZE;
    }

    memcpy(SegmentInfo, pEntry, sizeof(Elf64_Phdr));

    return STATUS_SUCCESS;
}
