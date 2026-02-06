#include "common_lib.h"
#include "syscall_if.h"
#include "um_lib_helper.h"
#include "cal_assembly.h"
#include "cal_seh.h"


STATUS
__main(
    DWORD       argc,
    char**      argv
)
{
    UM_HANDLE hThread;
    volatile QWORD test = 0;

    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    hThread = UM_INVALID_HANDLE_VALUE;

    __try
    {
        UmThreadCreate((PFUNC_ThreadStart) 0x70003203ULL, NULL, &hThread);

        // wait for the process to crash
        while(TRUE)
        {
            test += 1;
            AsmPause();
        }
    }
    __finally
    {

    }

    return STATUS_SUCCESS;
}