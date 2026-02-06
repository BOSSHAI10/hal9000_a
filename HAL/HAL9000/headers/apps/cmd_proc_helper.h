#pragma once

void
(CDECL CmdListProcesses)(
    IN      QWORD       NumberOfParameters
    );

void
(CDECL CmdProcessDump)(
    IN      QWORD       NumberOfParameters,
    IN      char*       PidString
    );

void
(CDECL CmdStartProcess)(
    IN          QWORD   NumberOfParameters,
    IN_Z        char*   ProcessPath
    );

void
(CDECL CmdTestProcess)(
    IN          QWORD       NumberOfParameters,
    IN_Z        char*       TestName
    );
