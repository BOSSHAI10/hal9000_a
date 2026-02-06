#pragma once

#include "cal_compiler.h"

#ifdef CAL_LLVM

#define static_assert(cond, msg) _Static_assert(cond, msg)

#endif
