#pragma once

#include "cal_annotate.h"
#include "cal_warning.h"

// warning C28208: Function was previously defined with a different parameter list
// warning C28250: Inconsistent annotation for Function: the prior instance has trailing unannotated parameters
// warning C28251: Inconsistent annotation for Function: this instance has trailing unannotated parameters.
// warning C28253: Inconsistent annotation for Function
MSVC_WARNING_DISABLE(28208)
MSVC_WARNING_DISABLE(28250)
MSVC_WARNING_DISABLE(28251)
MSVC_WARNING_DISABLE(28253)

typedef
void
(CDECL FUNC_GenericCommand)(
    IN      QWORD           NumberOfParameters,
    ...
    );

typedef FUNC_GenericCommand*    PFUNC_GenericCommand;
