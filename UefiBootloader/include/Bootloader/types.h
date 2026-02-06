#pragma once

#include <Uefi.h>

#define UINT32_MAX 4294967295U
#define UINT64_MAX 18446744073709551615ULL

#define UEFI_RUNTIME_SERVICES_ADDRESS 0xFFFF8A0000000000

#pragma pack(push, 1)

typedef struct 
{
    UINT32 DescriptorSize;
    UINT32 Count;
    UINT64 MapAddress;
} HAL_MEMORY_MAP;

typedef struct
{
    UINT32 PhysicalAddress; 
    UINT32 Size;
    CHAR8 Name[33];
} HAL_BOOT_MODULE;

typedef struct
{
    UINT64 Address;
    UINT32 Pitch;
    UINT32 Width;
    UINT32 Height;
    UINT32 BitsPerPixel;
    UINT32 RedFieldPosition;
    UINT32 RedMaskSize;
    UINT32 GreenFieldPosition;
    UINT32 GreenMaskSize;
    UINT32 BlueFieldPosition;
    UINT32 BlueMaskSize;
} HAL_FRAMEBUFFER;

#pragma pack(pop)
