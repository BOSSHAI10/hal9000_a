#include "HAL9000.h"
#include "cmd_sys_helper.h"
#include "ex.h"
#include "print.h"
#include "core.h"
#include "strutils.h"
#include "keyboard.h"
#include "acpi_interface.h"
#include "cal_annotate.h"

WARNING_PUSH

// warning C4212: nonstandard extension used: function declaration used ellipsis
MSVC_WARNING_DISABLE(4212)

// warning C4029: declared formal parameter list different from definition
MSVC_WARNING_DISABLE(4029)

void
(CDECL CmdDisplaySysInfo)(
    IN          QWORD       NumberOfParameters
    )
{
    SYSTEM_INFORMATION sysInfo;
    QWORD sizeInKB;
    QWORD frequencyMhz;
    QWORD uptimeInMs;

    ASSERT(NumberOfParameters == 0);

    ExGetSystemInformation(&sysInfo);

    sizeInKB = sysInfo.TotalPhysicalMemory / KB_SIZE;
    frequencyMhz = sysInfo.CpuFrequency / SEC_IN_US;
    uptimeInMs = sysInfo.SystemUptimeUs / MS_IN_US;

    printf("System memory: %U KB (%U MB)\n", sizeInKB, sizeInKB / KB_SIZE );
    printf("Highest physical memory: 0x%X\n", sysInfo.HighestPhysicalAddress );
    printf("CPU frequency: %u.%02u GHz\n", frequencyMhz / 1000, frequencyMhz % 1000 );
    printf("Uptime: %u.%03u sec\n", uptimeInMs / 1000, uptimeInMs % 1000 );
}

void
(CDECL CmdSetIdle)(
    IN          QWORD       NumberOfParameters,
    IN_Z        char*       SecondsString
    )
{
    DWORD noOfSeconds;

    ASSERT(NumberOfParameters == 1);

    atoi32(&noOfSeconds, SecondsString, BASE_TEN);

    if (0 == noOfSeconds)
    {
        pwarn("Idle period must differ from 0\n");
        return;
    }

    printf("Setting idle period to %u seconds\n", noOfSeconds);
    CoreSetIdlePeriod(noOfSeconds);
}

void
(CDECL CmdGetIdle)(
    IN          QWORD       NumberOfParameters
    )
{
    DWORD idlePeriod;

    ASSERT(NumberOfParameters == 0);

    idlePeriod = CoreSetIdlePeriod(0);
    printf("Idle period: %u seconds\n", idlePeriod );
}

void
(CDECL CmdResetSystem)(
    IN          QWORD       NumberOfParameters
    )
{
    ASSERT(NumberOfParameters == 0);

    KeyboardResetSystem();
}

void
(CDECL CmdShutdownSystem)(
    IN          QWORD       NumberOfParameters
    )
{
    ASSERT(NumberOfParameters == 0);

    AcpiShutdown();
}

WARNING_POP
