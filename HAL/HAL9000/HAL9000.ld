/*
    Linker script for HAL9000
*/
OUTPUT_FORMAT("elf64-x86-64")
ENTRY(__HalEntry)

PHDRS
{
    headers PT_LOAD FILEHDR PHDRS ;
    text PT_LOAD ;
    data PT_LOAD ;
    rodata PT_LOAD ;
}

SECTIONS
{
    /*
        HAL9000 Virtual Base
        We use the large code model so we do not get
        relocation errors from ld
    */
    . = 0xFFFF800001000000;

    .text 0xFFFF800001000400 : ALIGN(0x200)
    {
        *(.halboot)
        *(.text)
    } :text

    .data : ALIGN(0x1000)
    {
        *(.data)
        *(.data1)
        *(.bss)
    } :data

    .rodata : ALIGN(0x1000)
    {
        *(.rodata)
        *(.rodata1)
    } :rodata
}
