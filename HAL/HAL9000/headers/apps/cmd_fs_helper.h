#pragma once

#include "cal_annotate.h"

void
(CDECL CmdStatFile)(
    IN      QWORD       NumberOfParameters,
    IN_Z    char*       File
    );

void
(CDECL CmdMakeDirectory)(
    IN      QWORD       NumberOfParameters,
    IN_Z    char*       File
    );

void
(CDECL CmdMakeFile)(
    IN      QWORD       NumberOfParameters,
    IN_Z    char*       File
    );

void
(CDECL CmdListDirectory)(
    IN      QWORD       NumberOfParameters,
    IN_Z    char*       File,
    IN_Z    char*       Recursive
    );

void
(CDECL CmdReadFile)(
    IN      QWORD       NumberOfParameters,
    IN_Z    char*       File,
    IN_Z    char*       Async
    );

void
(CDECL CmdWriteFile)(
    IN      QWORD       NumberOfParameters,
    IN_Z    char*       File,
    IN_Z    char*       CharToWrite,
    IN_Z    char*       Extend,
    IN_Z    char*       Async
    );

void
(CDECL CmdSwap)(
    IN      QWORD       NumberOfParameters,
    IN_Z    char*       Operation,
    IN_Z    char*       OffsetString
    );
