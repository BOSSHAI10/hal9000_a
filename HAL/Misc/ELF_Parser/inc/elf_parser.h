#pragma once

#include "elf_exports.h"

STATUS
ElfRetrieveFileHeader(
    PVOID ImageBase,
    DWORD ImageSize,
    Elf64_Ehdr* FileHeader
    );

STATUS
ElfGetSegment(
    PVOID ImageBase,
    DWORD ImageSize,
    Elf64_Ehdr *FileHeader,
    DWORD SegmentIndex,
    Elf64_Phdr *SegmentInfo
    );
