#pragma once

#include "cal_compiler.h"

// GCC has problems with empty __VA_ARGS__, so we use a
// GNU extension ## has a special meaning in this case

#ifdef CAL_LLVM

#define VA_ARGS(...) , ##__VA_ARGS__

#else

#define VA_ARGS(...) , __VA_ARGS__

#endif