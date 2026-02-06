#pragma once

#include "cal_compiler.h"

// Main access point to SAL
// If we are MSVC use real sal.h
// else use our cal_sal.h taken from the mingw headers

#ifdef CAL_MSVC

#include <sal.h>

#else

#include "cal_saldef.h"

#endif
