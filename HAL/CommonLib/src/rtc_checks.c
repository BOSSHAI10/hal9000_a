#include "cal_annotate.h"
#include "cal_warning.h"

#ifdef CL_NO_RUNTIME_CHECKS
// warning C4206: nonstandard extension used: translation unit is empty
MSVC_WARNING_DISABLE(4206)
#else

#include "common_lib.h"

#define UNUSED_PATTERN     (DWORD)0xCCCCCCCCUL

#pragma pack(push,8)
typedef struct _RTC_vardesc
{
    // offsets from ESP
    DWORD addr;
    DWORD size;
    char *name;
} _RTC_vardesc;

typedef struct _RTC_framedesc
{
    DWORD           varCount;
    DWORD           __alignment;
    _RTC_vardesc*   variables;
} _RTC_framedesc;
#pragma pack(pop)

WARNING_PUSH
GNU_WARNING_IGNORE("-Winvalid-noreturn")

// NOT referenced anywhere in code
NO_RETURN
void
CDECL
_RTC_Shutdown(
    void
)
{
    NOT_REACHED;
}

// NOT referenced anywhere in code
NO_RETURN
void
CDECL
_RTC_InitBase(
    void
)
{
    NOT_REACHED;
}

WARNING_POP

// we are the ones responsible for checking if any buffer overflow occurred near
// the protected variables
void
FASTCALL
_RTC_CheckStackVars(
    PVOID           Rsp,
    _RTC_framedesc *_Fd
)
{
    DWORD i;

    for (i = 0; i < _Fd->varCount; ++i)
    {
        DWORD baseValue, endValue;

        PDWORD pBase = (PDWORD)PtrOffset(Rsp, _Fd->variables[i].addr);
        PDWORD pEnd = (PDWORD)PtrOffset(pBase, _Fd->variables[i].size);

        baseValue = *(pBase - 1);
        endValue = *pEnd;

        ASSERT_INFO(baseValue == UNUSED_PATTERN && endValue == UNUSED_PATTERN,
                    "Variable [%s]\n"
                    "Base value is 0x%x at 0x%X\n"
                    "End value is 0x%x at 0x%X\n"
                    "Both values should be 0x%x\n"
                    "RA at 0x%X\n",
                    _Fd->variables[i].name,
                    baseValue, pBase - 1,
                    endValue, pEnd,
                    UNUSED_PATTERN,
                    GET_RETURN_ADDRESS
        );
    }
}
#endif // CL_NO_RUNTIME_CHECKS
