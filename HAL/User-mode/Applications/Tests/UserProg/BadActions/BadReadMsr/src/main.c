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

    LOG("Congratulations - you have successfully read a MSR: 0x%X",
        AsmReadMsr(0xC0000100));
    LOG_ERROR("Should have terminated the process!");

    return STATUS_SUCCESS;
}