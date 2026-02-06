#pragma once

#include <Uefi.h>

void
LoaderPreInit(
    EFI_SYSTEM_TABLE *SystemTable,
    EFI_HANDLE ImageHandle
    );

void
LoaderInit(
    void
    );

void
LoadOperatingSystem(
    void
    );

void
StartOperatingSystem(
    void
    );
