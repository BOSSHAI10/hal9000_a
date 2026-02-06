#pragma once

#include <Uefi.h>

UINT32
ComputeCrc32(
    UINT8 *Data,
    UINT32 Length
    );
