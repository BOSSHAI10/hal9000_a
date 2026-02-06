#pragma once

#include <Uefi.h>

#include <Bootloader/types.h>

#pragma pack(push, 1)

#define HAL_BOOT_HEADER_MAGIC 0xFEE1DEAD
#define HAL_BOOT_MAGIC 0xC00010FF 
#define CRC32_REVERSED_POLYNOMIAL 0xEDB88320
#define BIOS_MAX_SERIAL_PORTS 4

typedef struct
{
    UINT32 Magic;
    UINT32 Crc32; 
    UINT32 LoadOffset;
    UINT32 PreferredLoadAddress; 
    UINT32 EntryAddress;
    UINT32 FramebufferWidth;
    UINT32 FramebufferHeight;
} HAL_BOOT_HEADER;

typedef struct
{
    UINT32 Magic;                  // 0
    UINT32 Crc32;                  // 4

    // Kernel information
    UINT32 KernelBaseAddress;      // 8
    UINT32 KernelSize;             // 12
     
    // ACPI RSDP
    UINT64 AcpiRsdp;               // 16

    // HAL Memory Map
    HAL_MEMORY_MAP MemoryMap;      // 24

    // Boot modules
    UINT32 BootModuleCount;        // 40
    // HAL_BOOT_MODULE*
    UINT32 BootModules;            // 44

    // HAL_FRAMEBUFFER
    HAL_FRAMEBUFFER Framebuffer;   // 48

    // EFI_RUNTIME_SERVICES
    UINT64 EfiRuntimeVirtualAddress;     // 96
    UINT64 EfiRuntimeDescriptorCount;    // 104
    UINT64 EfiRuntimeServices;        // 112

    // Serial Ports
    UINT16 SerialPorts[BIOS_MAX_SERIAL_PORTS]; // 120
    
    // Virtual address information
    UINT64 VirtualToPhysicalOffset;       // 128
    UINT64 VirtualDisplayAddress;         // 136

} HAL_BOOT_INFORMATION;

#pragma pack(pop)
