#pragma once

#include "cal_annotate.h"
#include "cal_assembly.h"

typedef BYTE APIC_ID;

void
CpuClearDirectionFlag(
    void
    );


INTR_STATE
CpuIntrGetState(
    void
    );

INTR_STATE
CpuIntrSetState(
    const      INTR_STATE         IntrState
    );

INTR_STATE
CpuIntrDisable(
    void
    );

INTR_STATE
CpuIntrEnable(
    void
    );

APIC_ID
CpuGetApicId(
    void
    );

BOOLEAN
CpuIsIntel(
    void
    );
