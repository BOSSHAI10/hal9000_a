#pragma once

C_HEADER_START

#include "cal_annotate.h"
#include "cal_vargs.h"
#include "cal_warning.h"
#include "native/string.h"

#ifndef SEPARATOR
#ifdef CAL_MSVC
#define SEPARATOR '\\'
#else
#define SEPARATOR '/'
#endif
#endif

//******************************************************************************
// Function:     FUNC_AssertFunction
// Description:  Function which is responsible for handling a system assertion
//               failure.
// Returns:      void
// Parameter:    IN_Z            char*           Message
//******************************************************************************
typedef
void
(CDECL FUNC_AssertFunction)(
    IN_Z            char*           Message
    );

typedef FUNC_AssertFunction*        PFUNC_AssertFunction;

/// These ASSERT* functions cannot be used before
/// AssertSetFunction is called

// The ASSERT macros validate a specified condition, in case the
// condition does not hold the registered FUNC_AssertFunction is called
#ifndef ASSERT
// GCC has problems with the empty string
// #define ASSERT(Cond)                ASSERT_INFO((Cond),"")
#define ASSERT(Cond)         if((Cond)){} else                                                                                                  \
                             {                                                                                                                 \
                                AssertInfo( "[ASSERT][%s][%d]Condition: (" #Cond ") failed\n", cl_strrchr(__FILE__, SEPARATOR) + 1, __LINE__);      \
                             }
#endif // ASSERT

#define ASSERT_INFO(Cond,Msg,...)   if((Cond)){} else                                                                                                    \
                                    {                                                                                                                    \
                                        AssertInfo( "[ASSERT][%s][%d]Condition: (" #Cond ") failed\n" Msg, cl_strrchr(__FILE__, SEPARATOR) + 1, __LINE__ VA_ARGS(__VA_ARGS__) );      \
                                    }

#define NOT_REACHED                  MSVC_WARNING_SUPPRESS(4127) ASSERT(FALSE)

void
AssertInfo(
    IN_Z            char*       Message,
    ...
    );

void
AssertSetFunction(
    IN              PFUNC_AssertFunction    AssertFunction
    );

#ifndef _COMMONLIB_NO_LOCKS_
REQUIRES_EXCL_LOCK(m_assertLock)
RELEASES_EXCL_AND_NON_REENTRANT_LOCK(m_assertLock)

#ifndef COMMONLIB_LIB_IMPL
// Warning C28285 For function 'AssertFreeLock' 'return' syntax error
// This is suppressed only for external projects because they have no idea who m_assertLock is
MSVC_WARNING_SUPPRESS(28285)
#endif
void
AssertFreeLock(
    void
    );
#endif // _COMMONLIB_NO_LOCKS_
C_HEADER_END
