#pragma once

#include "cal_compiler.h"

// Macro black magic for ignoring warnings

#ifdef CAL_MSVC

#define WARNING_PUSH DO_PRAGMA(warning(push))
#define WARNING_POP  DO_PRAGMA(warning(pop))

#define MSVC_WARNING_SUPPRESS(x) DO_PRAGMA(warning(suppress: ## x))
#define MSVC_WARNING_DISABLE(x) DO_PRAGMA(warning(disable: ## x))

#define GNU_WARNING_IGNORE(x)
#define GNU_WARNING_ERROR(x)

#else

#define WARNING_PUSH DO_PRAGMA(GCC diagnostic push)
#define WARNING_POP DO_PRAGMA(GCC diagnostic pop)

#define MSVC_WARNING_SUPPRESS(x)
#define MSVC_WARNING_DISABLE(x)

#define GNU_WARNING_IGNORE(x) DO_PRAGMA(GCC diagnostic ignored x)
#define GNU_WARNING_ERROR(x) DO_PRAGMA(GCC diagnostic error x)

#endif