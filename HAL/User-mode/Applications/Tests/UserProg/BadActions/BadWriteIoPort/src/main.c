#include "common_lib.h"
#include "syscall_if.h"
#include "um_lib_helper.h"
#include "cal_assembly.h"

STATUS
__main(
    DWORD       argc,
    char**      argv
)
{
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    AsmOutByte(0x60, 0x5);
    LOG_ERROR("Should have terminated the process!");

    return STATUS_SUCCESS;
}