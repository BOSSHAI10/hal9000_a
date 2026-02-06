#pragma once

#include "common_lib.h"
#include "uefi.h"

#pragma pack(push,1)

// it is not physically possible to have more than 4
// in the BDA
#define     BIOS_MAX_NO_OF_SERIAL_PORTS     4

typedef struct _HAL_MEMORY_MAP 
{
    DWORD DescriptorSize;
    DWORD Count;
    QWORD MapAddress;
} HAL_MEMORY_MAP;

typedef struct _HAL_BOOT_MODULE
{
    DWORD PhysicalAddress; 
    DWORD Size;
    char Name[33];
} HAL_BOOT_MODULE;

typedef struct _HAL_FRAMEBUFFER
{
    QWORD Address;
    DWORD Pitch;
    DWORD Width;
    DWORD Height;
    DWORD BitsPerPixel;
    DWORD RedFieldPosition;
    DWORD RedMaskSize;
    DWORD GreenFieldPosition;
    DWORD GreenMaskSize;
    DWORD BlueFieldPosition;
    DWORD BlueMaskSize;
} HAL_FRAMEBUFFER;

typedef struct _HAL_BOOT_INFORMATION
{
    DWORD Magic;
    // CRC32 without last two fields, which are
    // completed by HAL
    DWORD Crc32;

    // Kernel information
    DWORD KernelBaseAddress;
    DWORD KernelSize;
     
    // ACPI RSDP
    QWORD AcpiRsdp;

    // HAL Memory Map
    HAL_MEMORY_MAP MemoryMap;

    // Boot modules
    DWORD BootModuleCount;
    // HAL_BOOT_MODULE*
    DWORD BootModules;

    // HAL_FRAMEBUFFER
    HAL_FRAMEBUFFER Framebuffer;

    // EFI_RUNTIME_SERVICES
    UINT64 EfiRuntimeVirtualAddress;
    UINT64 EfiRuntimeDescriptorCount;
    UINT64 EfiRuntimeServices;

    // Serial Ports
    WORD SerialPorts[BIOS_MAX_NO_OF_SERIAL_PORTS];
    
    // Virtual address information
    UINT64 VirtualToPhysicalOffset;
    UINT64 VirtualDisplayAddress;

} HAL_BOOT_INFORMATION;

STATUS
ValidateHalBootInformation(
    IN HAL_BOOT_INFORMATION *BootInformation
    );

#pragma pack(pop)