#pragma once

#include "cal_annotate.h"

WORD
PitSetTimer(
    IN      DWORD       Microseconds,
    IN      BOOLEAN     Periodic
    );

void
PitStartTimer(
    void
    );

void
PitWaitTimer(
    void
    );

void
PitSleep(
    IN      DWORD       Microseconds
    );

DEPRECATED
WORD
PitGetTimerCount(
    IN      BOOLEAN     Periodic
    );

