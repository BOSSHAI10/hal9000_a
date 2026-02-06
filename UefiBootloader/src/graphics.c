#include <Bootloader/graphics.h>

EFI_GRAPHICS_OUTPUT_PROTOCOL*
GetGraphicsProtocol(
    EFI_SYSTEM_TABLE *ST
    )
{
    EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;       
    EFI_STATUS status = ST->BootServices->LocateProtocol(&gopGuid, NULL, (void **) &gop);
    if(EFI_ERROR(status)) 
        return NULL;
    return gop;
}

// Dumb way
UINT32
ObtainClosestGraphicsMode(
    EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP,
    UINT32 Width,
    UINT32 Height,
    HAL_FRAMEBUFFER *Framebuffer
    ) 
{
    UINT32 maxMode = GOP->Mode->MaxMode;
    UINTN sizeInfo;
    UINT32 finalMode = UINT32_MAX;
    UINT32 height, width;
    width = height = UINT32_MAX;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
    for (int i = 0;i < maxMode;i++) {
        GOP->QueryMode(GOP, i, &sizeInfo, &info);
        if (info->VerticalResolution > Height && 
           info->HorizontalResolution > Width &&
           info->PixelFormat == PixelBlueGreenRedReserved8BitPerColor
            ) 
            {
                if (height > info->VerticalResolution && width > info->HorizontalResolution)
                {
                    height = info->VerticalResolution;
                    width = info->HorizontalResolution;
                    finalMode = i; 
                }
            }
        }

    if (finalMode == UINT32_MAX)
        return UINT32_MAX;

    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *mode;
    GOP->QueryMode(GOP, finalMode, &sizeInfo, &mode);

    Framebuffer->Address = GOP->Mode->FrameBufferBase;
    Framebuffer->Pitch = mode->PixelsPerScanLine * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL);
    Framebuffer->Width = mode->HorizontalResolution;
    Framebuffer->Height = mode->VerticalResolution;
    Framebuffer->BitsPerPixel = sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL) * 8;
    // 00000000 RRRRRRRR GGGGGGGG BBBBBBBB
    Framebuffer->RedFieldPosition = 16; 
    Framebuffer->RedMaskSize = 8;
    Framebuffer->GreenFieldPosition = 8;
    Framebuffer->GreenMaskSize = 8;
    Framebuffer->BlueFieldPosition = 0;
    Framebuffer->BlueMaskSize = 8;

    return finalMode;
}

EFI_STATUS
SetGraphicsMode(
    EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP,
    UINT32 Mode
    )
{
    return GOP->SetMode(GOP, Mode);
}
