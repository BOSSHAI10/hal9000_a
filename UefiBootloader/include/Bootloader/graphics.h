#pragma once

#include <Uefi.h>
#include <Protocol/GraphicsOutput.h>

#include <Bootloader/types.h>

EFI_GRAPHICS_OUTPUT_PROTOCOL*
GetGraphicsProtocol(
    EFI_SYSTEM_TABLE *ST
    );

UINT32 ObtainClosestGraphicsMode(
    EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP,
    UINT32 Width,
    UINT32 Height,
    HAL_FRAMEBUFFER *Framebuffer
    );
 
EFI_STATUS
SetGraphicsMode(
    EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP,
    UINT32 Mode
    );
