#pragma once

C_HEADER_START

#include "cal_types.h"
#include "cal_warning.h"

WARNING_PUSH

// warning C4142: 'DWORD': benign redefinition of type
MSVC_WARNING_DISABLE(4142)

#ifndef TRUE
#define TRUE                        ( 1 == 1 )
#endif

#ifndef FALSE
#define FALSE                       ( 1 == 0 )
#endif

#ifndef NULL
#define NULL                        ( (void*) 0 )
#endif

#define MAX_NIBBLE                  0xFU
#define MAX_BYTE                    0xFFU
#define MAX_WORD                    0xFFFFU
#define MAX_DWORD                   0xFFFFFFFFUL
#define MAX_QWORD                   0xFFFFFFFFFFFFFFFFULL

// VMX operation
typedef BYTE                VMX_RESULT;

#ifndef CL_DO_NOT_DEFINE_PHYSICAL_ADDRESS
// physical memory address
typedef PVOID               PHYSICAL_ADDRESS;
#endif // CL_DO_NOT_DEFINE_PHYSICAL_ADDRESS

typedef volatile BYTE       VOL_BYTE;
typedef volatile WORD       VOL_WORD;
typedef volatile DWORD      VOL_DWORD;
typedef volatile QWORD      VOL_QWORD;

WARNING_POP

C_HEADER_END
