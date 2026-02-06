#pragma once

#include <Uefi.h>

EFI_STATUS 
PrepareConsole(
    EFI_SYSTEM_TABLE *ST
    );

EFI_STATUS 
ClearScreen(
    EFI_SYSTEM_TABLE *ST
    );

EFI_STATUS
PrintString(
    EFI_SYSTEM_TABLE *ST, 
    UINTN Color, 
    CHAR16 *String
    );

EFI_STATUS PrintIntegerInDecimal(
    EFI_SYSTEM_TABLE *ST, 
    UINTN Color, 
    UINTN Integer
    ); 

EFI_STATUS 
PrintIntegerInHexadecimal(
    EFI_SYSTEM_TABLE *ST,
    UINTN Color,
    UINTN Integer
    );

EFI_STATUS
PrintIntegerWithName(
    EFI_SYSTEM_TABLE *ST,
    UINTN Color,
    CHAR16 *Name,
    UINT64 Integer
    );

EFI_STATUS 
ClearScreen(
    EFI_SYSTEM_TABLE *ST
    );

EFI_STATUS 
WaitForKeyPress(
    EFI_SYSTEM_TABLE *ST
    );
