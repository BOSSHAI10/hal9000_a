#pragma once

#include "cal_compiler.h"

#ifdef CAL_MSVC

#define ALIGN(x) __declspec(align(x))
#define ALWAYS_INLINE __forceinline
#define NO_RETURN __declspec(noreturn)
#define DEPRECATED __declspec(deprecated)

#define CDECL __cdecl
#define FASTCALL __fastcall
#define STDCALL  __stdcall
#define MS_ABI
#define NAKED

#else

#define ALIGN(x) __attribute__((aligned(x)))
#define ALWAYS_INLINE inline __attribute__((always_inline))
#define NO_RETURN __attribute__((noreturn))
#define DEPRECATED __attribute__((deprecated))

// GCC ignores this attribute and generates a warning
// #define CDECL __attribute__((cdecl))
// #define FASTCALL __attribute__((fastcall))

#define CDECL
#define FASTCALL
#define STDCALL __attribute__((stdcall))
#define MS_ABI __attribute__((ms_abi))
#define NAKED __attribute__((naked))

#endif
