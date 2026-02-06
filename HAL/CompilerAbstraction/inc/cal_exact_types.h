#pragma once

#include "cal_compiler.h"

// Inspired by GCC's stdint.h, I decided no to use it even though
// freestanding code is allowed to make use of it
// I don't like the idea of anything std in HAL code, so I redefine these types here

// Signed types
#ifdef CAL_MSVC
typedef __int8 int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;
#else
typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;
#endif

// Unsigned types
#ifdef CAL_MSVC
typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
#else
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
#endif

// Integer limits (taken from stdint.h, did not verify them)

#define INT8_MIN (-128)
#define INT16_MIN (-32768)
#define INT32_MIN (-2147483647 - 1)
#define INT64_MIN  (-9223372036854775807LL - 1)

#define INT8_MAX 127
#define INT16_MAX 32767
#define INT32_MAX 2147483647
#define INT64_MAX 9223372036854775807LL

#define UINT8_MAX 255
#define UINT16_MAX 65535
#define UINT32_MAX 0xffffffffU
#define UINT64_MAX 0xffffffffffffffffULL

// Other types

typedef int64_t ssize_t;
typedef uint64_t size_t;

// Fixed width constants

#define UINT8_C(x) ((uint8_t) (x))
#define UINT16_C(x) ((uint16_t) (x))
#define UINT32_C(x) ((uint32_t) (x))
#define UINT64_C(x) ((uint64_t) (x))

#define INT8_C(x) ((int8_t) (x))
#define INT16_C(x) ((int16_t) (x))
#define INT32_C(x) ((int32_t) (x))
#define INT64_C(x) ((int64_t) (x))
