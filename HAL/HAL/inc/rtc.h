#pragma once

#include "cal_annotate.h"
#include "cal_assembly.h"

void
RtcInit(
    OUT_OPT     QWORD*          TscFrequency
    );

void
RtcAcknowledgeTimerInterrupt(
    void
    );

ALWAYS_INLINE
QWORD
RtcGetTickCount(
    void
    )
{
    AsmLfence();
    return AsmRdtsc();
}