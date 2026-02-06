#include "bootinfo.h"
#include "HAL9000.h"
#include "crc32.h"

#define HAL_BOOT_MAGIC 0xC00010FF

STATUS
ValidateHalBootInformation(
    IN HAL_BOOT_INFORMATION *BootInformation
    )
{
    if (BootInformation == NULL)
    {
        return STATUS_UNSUCCESSFUL;
    }

    if (BootInformation->Magic != HAL_BOOT_MAGIC)
    {
        return STATUS_UNSUCCESSFUL;
    }

    HAL_BOOT_INFORMATION BootInformationCopy;
    memcpy(&BootInformationCopy, BootInformation, sizeof(HAL_BOOT_INFORMATION));
    BootInformationCopy.Crc32 = 0;
    
    DWORD ExpectedCrc32 = ComputeCrc32((const BYTE *) &BootInformationCopy, sizeof(HAL_BOOT_INFORMATION) - 16);

    if (BootInformation->Crc32 != ExpectedCrc32)
    {
        return STATUS_UNSUCCESSFUL;
    }

    return STATUS_SUCCESS;
}
