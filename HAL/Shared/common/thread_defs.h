#pragma once

#include "cal_annotate.h"

typedef QWORD       TID, *PTID;

typedef enum _THREAD_PRIORITY
{
    ThreadPriorityLowest            = 0,
    ThreadPriorityDefault           = 16,
    ThreadPriorityMaximum           = 31,
    ThreadPriorityReserved          = ThreadPriorityMaximum + 1
} THREAD_PRIORITY;

typedef struct _THREAD* PTHREAD;

typedef
STATUS
(CDECL MS_ABI FUNC_ThreadStart)(
    IN_OPT      PVOID       Context
    );

typedef FUNC_ThreadStart*   PFUNC_ThreadStart;