#include "common_lib.h"
#include "cpu_utils.h"
#include "cpuid_leaf.h"
#include "register.h"

void
CpuClearDirectionFlag(
    void
    )
{
    AsmWriteEflags(AsmReadEflags() & (~RFLAGS_DIRECTION_BIT));
}

INTR_STATE
CpuIntrGetState(
    void
    )
{
    return IsBooleanFlagOn(AsmReadEflags(), RFLAGS_INTERRUPT_FLAG_BIT);
}

INTR_STATE
CpuIntrSetState(
    const      INTR_STATE         IntrState
    )
{
    QWORD rFlags = AsmReadEflags();
    QWORD newFlags = IntrState ? ( rFlags | RFLAGS_INTERRUPT_FLAG_BIT ) : ( rFlags & ( ~RFLAGS_INTERRUPT_FLAG_BIT));

    AsmWriteEflags(newFlags);

    return IsBooleanFlagOn(rFlags, RFLAGS_INTERRUPT_FLAG_BIT);
}

INTR_STATE
CpuIntrDisable(
    void
    )
{
    return CpuIntrSetState(FALSE);
}

INTR_STATE
CpuIntrEnable(
    void
    )
{
    return CpuIntrSetState(TRUE);
}

APIC_ID
CpuGetApicId(
    void
    )
{
    CPUID_INFO cpuId;

    AsmCpuid(cpuId.values, CpuidIdxFeatureInformation);

    return cpuId.FeatureInformation.ebx.ApicId;
}

BOOLEAN
CpuIsIntel(
    void
    )
{
    CPUID_INFO cpuId;

    AsmCpuid(cpuId.values, CpuidIdxBasicInformation);

    return ( cpuId.ebx == 'uneG' &&
             cpuId.edx == 'Ieni' &&
             cpuId.ecx == 'letn' );
}
