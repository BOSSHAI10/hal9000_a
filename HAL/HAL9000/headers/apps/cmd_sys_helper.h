#pragma once

#include "cal_annotate.h"

void
(CDECL CmdDisplaySysInfo)(
    IN          QWORD       NumberOfParameters
    );

void
(CDECL CmdSetIdle)(
    IN          QWORD       NumberOfParameters,
    IN_Z        char*       SecondsString
    );

void
(CDECL CmdGetIdle)(
    IN          QWORD       NumberOfParameters
    );

void
(CDECL CmdResetSystem)(
    IN          QWORD       NumberOfParameters
    );

void
(CDECL CmdShutdownSystem)(
    IN          QWORD       NumberOfParameters
    );