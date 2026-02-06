#pragma once

// To load ELF programs we just need these definitions

#pragma pack(push, 1)

// Elf64 Data Types

typedef unsigned long long      Elf64_Addr;
typedef unsigned long long      Elf64_Off;
typedef unsigned short          Elf64_Half;
typedef unsigned int            Elf64_Word;
typedef int                     Elf64_Sword;
typedef unsigned long long      Elf64_Xword;
typedef long long               Elf64_Sxword;


// Elf64 Identification macros

#define EI_MAG0         0    /* File identification */
#define EI_MAG1         1
#define EI_MAG2         2
#define EI_MAG3         3
#define EI_CLASS        4    /* File class */
#define EI_DATA         5    /* Data encoding */
#define EI_VERSION      6    /* File version */
#define EI_OSABI        7    /* OS/ABI identification */
#define EI_ABIVERSION   8    /* ABI version */
#define EI_PAD          9    /* Start of padding bytes */
#define EI_NIDENT       16   /* Size of e_ident[] */


// Elf64 Identification values

#define ELFMAG0         0x7F
#define ELFMAG1         'E'
#define ELFMAG2         'L'
#define ELFMAG3         'F'

#define EV_CURRENT      1    /* Current verion */

#define ELFCLASSNONE    1    /* Invalid class */
#define ELFCLASS32      1    /* 32-bit objects */
#define ELFCLASS64      2    /* 64-bit objects */

#define ELFDATANONE     1    /* Invalid class */
#define ELFDATA2LSB     1    /* Object file data structures are little-endian */
#define ELFDATA2MSB     2    /* Object file data structures are big-endian */

#define ELFOSABI_SYSV   0    /* System V ABI */

#define ET_NONE         0    /* No file type */
#define ET_REL          1    /* Relocatable object file */
#define ET_EXEC         2    /* Executable file */
#define ET_DYN          3    /* Shared object file */
#define ET_CORE         4    /* Core file */

#define EM_386          3    /* Intel 80386 */
#define EM_X86_64       62   /* AMD x86-64 architecture */

// Elf64 file header

typedef struct
{
    unsigned char        e_ident[16];   /* ELF identification */
    Elf64_Half           e_type;        /* Object file type */
    Elf64_Half           e_machine;     /* Machine type */
    Elf64_Word           e_version;     /* Object file version */
    Elf64_Addr           e_entry;       /* Entry point address */
    Elf64_Off            e_phoff;       /* Program header offset */
    Elf64_Off            e_shoff;       /* Section header ofset */
    Elf64_Word           e_flags;       /* Processor-specific flags */
    Elf64_Half           e_ehsize;      /* ELF header size */
    Elf64_Half           e_phentsize;   /* Size of program header entry */
    Elf64_Half           e_phnum;       /* Number of program header entries */
    Elf64_Half           e_shentsize;   /* Size of section header entry */
    Elf64_Half           e_shnum;       /* Number of section header entries */
    Elf64_Half           e_shstrndx;    /* Section name string table index */
} Elf64_Ehdr;

// Elf64 program header macros

#define PT_NULL          0   /* Unused entry */
#define PT_LOAD          1   /* Loadable segment */
#define PT_DYNAMIC       2   /* Dynamic linking tables */
#define PT_INTERP        3   /* Program interpreter path name */
#define PT_NOTE          4   /* Notes sections */
#define PT_SHLIB         5   /* Reserved */
#define PT_PHDR          6   /* Program header table */
#define PT_TLS           7   /* Thread-local storage template */

#define PF_X             1   /* Execute permission */
#define PF_W             2   /* Write permission */
#define PF_R             4   /* Read permission */

// Elf64 program header

typedef struct 
{
    Elf64_Word           p_type;        /* Type of segment */
    Elf64_Word           p_flags;       /* Segment attributes */
    Elf64_Off            p_off;         /* Offset in file */
    Elf64_Addr           p_vaddr;       /* Virtual address in memory */
    Elf64_Addr           p_paddr;       /* Reserved */
    Elf64_Xword          p_filesz;      /* Size of segment in file */
    Elf64_Xword          p_memsz;       /* Size of semgent in memory */
    Elf64_Xword          p_align;       /* Alignment of segment */
} Elf64_Phdr;

#pragma pack(pop)
