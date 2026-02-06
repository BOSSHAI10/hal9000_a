#pragma once

void
SystemPreinit(
    IN  HAL_BOOT_INFORMATION*     BootInformation
    );

STATUS
SystemInit(
    IN  HAL_BOOT_INFORMATION*     BootInformation
    );

void
SystemUninit(
    void
    );