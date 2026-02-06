#pragma once

#include "cal_compiler.h"

#ifdef CAL_MSVC

#define PUSH_OPTIONS
#define POP_OPTIONS

#define NO_OPTIMIZE DO_PRAGMA(optimize("", off))
#define OPTIMIZE DO_PRAGMA(optimize("", on))

#else

// Clang does not support them
// #define PUSH_OPTIONS DO_PRAGMA(GCC push_options)
// #define POP_OPTIONS DO_PRAGMA(GCC pop_options)

#define NO_OPTIMIZE DO_PRAGMA(clang optimize off)
#define OPTIMIZE DO_PRAGMA(clang optimize on);

#endif
