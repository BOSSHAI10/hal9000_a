#pragma once

#include "cal_compiler.h"

// Very UGLY hack to get around SEH
// Be open please and consider this a temporary solution
// Next time do not USE __try __finally, on bare metal there is no replacement...

#ifdef CAL_LLVM

#define __try
#define __finally Cleanup:
#define __leave goto Cleanup

#endif
