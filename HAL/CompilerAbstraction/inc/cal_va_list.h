#pragma once

#include "cal_compiler.h"

#ifdef CAL_MSVC

#ifndef _VA_LIST_DEFINED
#define _VA_LIST_DEFINED
typedef PBYTE               va_list;
#endif

#define STACKITEM_SIZE      sizeof(PVOID)

// Initializes the va_list
#define va_start(List,LastArg)     \
            ((List)=((va_list)&(LastArg) + STACKITEM_SIZE))

// Retrieves the value of the next argument
// And increases the List pointer
#define va_arg(List, Type)	\
	((List) += STACKITEM_SIZE, *((Type *)((List) - STACKITEM_SIZE)))

#else

// We know for use in case GNU that va_list is not defined
// Use compiler intrinsic for variadic functions

typedef __builtin_va_list va_list;

#define va_start(List, LastArg) __builtin_va_start(List, LastArg)

#define va_end(List) __builtin_va_end(List)

#define va_arg(List, Type) __builtin_va_arg(List, Type)

#endif
