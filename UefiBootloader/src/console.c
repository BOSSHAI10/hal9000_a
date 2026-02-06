#include <Bootloader/console.h>

EFI_STATUS 
PrepareConsole(
    EFI_SYSTEM_TABLE *ST
    ) 
{
    UINTN maxMode = ST->ConOut->Mode->MaxMode;
    UINTN finalMode;
    UINTN col = 0;
    UINTN row = 0;
    UINTN columns, rows;
    for (UINTN i = 0;i < maxMode;i++) 
    {
        ST->ConOut->QueryMode(ST->ConOut, i, &columns, &rows);
        if (rows > row && columns > col)
        {
            row = rows;
            col = columns;
            finalMode = i;
        }
    }
    return ST->ConOut->SetMode(ST->ConOut, finalMode);
}

EFI_STATUS 
ClearScreen(
    EFI_SYSTEM_TABLE *ST
    ) 
{
    return ST->ConOut->ClearScreen(ST->ConOut);
}

EFI_STATUS 
PrintString(
    EFI_SYSTEM_TABLE *ST,
    UINTN Color, 
    CHAR16 *String
    ) 
{
    ST->ConOut->SetAttribute(ST->ConOut, Color);
    return ST->ConOut->OutputString(ST->ConOut, String);
}

EFI_STATUS PrintIntegerInDecimal(
    EFI_SYSTEM_TABLE *ST, 
    UINTN Color, 
    UINTN Integer
    ) 
{
    UINTN n = Integer;
    UINTN digit = 1;
    while (n > 9) 
    {
        digit *= 10;
        n /= 10; 
    }
    CHAR16 buff[2];
    buff[1] = 0;
    while (digit) 
    {
        buff[0] = (CHAR16) ((Integer / digit) % 10 + L'0');
        PrintString(ST, Color, buff);
        digit /= 10;
    }
    return EFI_SUCCESS;
}

EFI_STATUS 
PrintIntegerInHexadecimal(
    EFI_SYSTEM_TABLE *ST, 
    UINTN Color, 
    UINTN Integer
    ) 
{
    UINTN currentShift = 60;
    UINTN digitCount = 0;
    UINT8 firstDigit = 0;
    CHAR16 buff[2];
    buff[0] = L'0';
    buff[1] = 0;
    PrintString(ST, Color, buff);
    buff[0] = L'x';
    PrintString(ST, Color, buff);
    if (Integer == 0) 
    {
        buff[0] = L'0';
        PrintString(ST, Color, buff);
        return EFI_SUCCESS;
    }
    while (digitCount < 16) 
    {
        UINTN digit = (((UINT64) 0xF << currentShift) & Integer) >> currentShift;
        if (!firstDigit && digit)
            firstDigit = 1;
        if (firstDigit)
        {
            if(digit < 10)
                buff[0] = (CHAR16) (digit + L'0');
            else
                buff[0] = (CHAR16) ((digit - 10)  + L'A');
            PrintString(ST, Color, buff);
        }
        currentShift -= 4;
        digitCount++;
    } 
    return EFI_SUCCESS;
}

EFI_STATUS
PrintIntegerWithName(
    EFI_SYSTEM_TABLE *ST,
    UINTN Color,
    CHAR16 *Name,
    UINT64 Integer
    )
{
    PrintString(ST, Color, Name);
    PrintString(ST, Color, L": ");
    PrintIntegerInHexadecimal(ST, Color, Integer);
    PrintString(ST, Color, L"\r\n");
    return EFI_SUCCESS;
}

EFI_STATUS 
WaitForKeyPress(
    EFI_SYSTEM_TABLE *ST
    ) 
{
    EFI_INPUT_KEY key;
    EFI_STATUS status = ST->ConIn->Reset(ST->ConIn, FALSE);
    if (EFI_ERROR(status))
        return status;
    while ((status = ST->ConIn->ReadKeyStroke(ST->ConIn, &key)) == EFI_NOT_READY);
    return status;
}
