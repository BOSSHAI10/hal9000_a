#pragma once

#include "cal_compiler.h"

#ifdef CAL_MSVC

#define IntrinAddressOfReturnAddress _AddressOfReturnAddress()

#else

#define IntrinAddressOfReturnAddress ((void *) (((int64_t *) __builtin_frame_address(0)) - 1))

#endif
