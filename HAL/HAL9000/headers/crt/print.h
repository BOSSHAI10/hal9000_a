#pragma once

#include "cal_annotate.h"
#include "display.h"

typedef
void
(CDECL FUNC_PrintFunction)(
    IN_Z    char*   Format,
    ...
    );

typedef FUNC_PrintFunction*     PFUNC_PrintFunction;

void
printSystemPreinit(
    IN      PDISPLAY_INFORMATION  DisplayInformation
    );

void
perror(
    IN_Z    char*   Format,
    ...
    );

void
pwarn(
    IN_Z    char*   Format,
    ...
    );

void
printf(
    IN_Z    char*   Format,
    ...
    );

void
printColor(
    IN      BYTE    Color,
    IN_Z    char*   Format,
    ...
    );