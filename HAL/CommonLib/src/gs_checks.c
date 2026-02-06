#include "cal_annotate.h"
#include "cal_warning.h"

#ifdef CL_NO_RUNTIME_CHECKS

// warning C4206: nonstandard extension used: translation unit is empty
MSVC_WARNING_DISABLE(4206)
#else
#include "common_lib.h"

WARNING_PUSH
GNU_WARNING_IGNORE("-Winvalid-noreturn")

#define DEFAULT_SECURITY_COOKIE_VALUE       (QWORD)0xBEEF03012497EC03ULL

/// TODO: should we initialize the cookie at runtime?
const UINT64 __security_cookie = DEFAULT_SECURITY_COOKIE_VALUE;
const UINT64 __stack_chk_guard = DEFAULT_SECURITY_COOKIE_VALUE;

// called when a buffer bound check fails
// E.g: char buf[10] buf[10] = 'A';
// From what I've seen in the disassembly no parameters
// passed => we can only print the location where the
// instruction occurred
NO_RETURN
void
__report_rangecheckfailure(
    void
)
{
    // warning C4127: conditional expression is constant
MSVC_WARNING_SUPPRESS(4127)
    ASSERT_INFO(FALSE, "RA is 0x%X\n", GET_RETURN_ADDRESS);
}

// NOT referenced anywhere in code
NO_RETURN
void
__GSHandlerCheck_SEH(
    void
)
{
    NOT_REACHED;
}

// NOT referenced anywhere in code
NO_RETURN
void
__GSHandlerCheck(
    void
)
{
    NOT_REACHED;
}

// Called for each function which uses a cookie
NO_RETURN
void
CDECL
__report_cookie_corruption(
    IN UINT64 StackCookie
)
{
    ASSERT_INFO(StackCookie == __security_cookie,
                "Security cookie is 0x%X but should have been 0x%X. RA is 0x%X\n",
                StackCookie, __security_cookie, GET_RETURN_ADDRESS);
}

NO_RETURN
void
__stack_chk_fail(
    void
    )
{   
    ASSERT_INFO(FALSE, "Security cookie is damaged. RA is 0x%X\n",
                GET_RETURN_ADDRESS);
}

WARNING_POP

#endif // CL_NO_RUNTIME_CHECKS
