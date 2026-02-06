/*
    Linker script for HAL9000 ELF Applications
    This will align everything correctly, we just need
    to copy-paste the file in memory and map it
    based on the program headers and we are done.
*/
OUTPUT_FORMAT("elf64-x86-64")
ENTRY(__start)

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
        16MB
    */
    . = 0x1000000;

    .text : ALIGN(0x1000) 
    {
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
