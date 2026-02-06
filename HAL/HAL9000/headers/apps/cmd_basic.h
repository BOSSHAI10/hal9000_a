#pragma once

#include "cal_annotate.h"

void
CmdPrintVolumeInformation(
    IN      QWORD           NumberOfParameters
    );

void
CmdInfiniteRecursion(
    IN      QWORD           NumberOfParameters
    );

void
CmdRtcFail(
    IN      QWORD           NumberOfParameters
    );

void
CmdRangeFail(
    IN      QWORD           NumberOfParameters
    );

void
(CDECL CmdBiteCookie)(
    IN      QWORD           NumberOfParameters
    );

void
(CDECL CmdLogSetState)(
    IN      QWORD           NumberOfParameters,
    IN      char*           LogState
    );

void
(CDECL CmdSetLogLevel)(
    IN      QWORD           NumberOfParameters,
    IN      char*           LogLevelString
    );

void
(CDECL CmdSetLogComponents)(
    IN      QWORD           NumberOfParameters,
    IN      char*           LogComponentsString
    );

void
(CDECL CmdClearScreen)(
    IN          QWORD       NumberOfParameters
    );

void
(CDECL CmdRunAllFunctionalTests)(
    IN          QWORD       NumberOfParameters
    );

void
(CDECL CmdRunAllPerformanceTests)(
    IN          QWORD       NumberOfParameters
    );
