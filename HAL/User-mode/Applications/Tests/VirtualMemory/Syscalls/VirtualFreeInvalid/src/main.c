#include "common_lib.h"
#include "syscall_if.h"
#include "um_lib_helper.h"
#include "cal_intrin.h"

STATUS
__main(
    DWORD       argc,
    char**      argv
)
{
    STATUS status;

    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    status = SyscallVirtualFree(IntrinAddressOfReturnAddress,
                                0,
                                VMM_FREE_TYPE_DECOMMIT | VMM_FREE_TYPE_RELEASE);
    if (SUCCEEDED(status))
    {
        LOG_ERROR("SyscallVirtualFree succeeded for memory which was not allocated through SyscallVirtualAlloc!\n");
        return status;
    }

    status = SyscallVirtualFree(IntrinAddressOfReturnAddress,
                                0,
                                VMM_FREE_TYPE_DECOMMIT);
    if (SUCCEEDED(status))
    {
        LOG_ERROR("SyscallVirtualFree succeeded for memory which was not allocated through SyscallVirtualAlloc!\n");
        return status;
    }

    status = SyscallVirtualFree(IntrinAddressOfReturnAddress,
                                0,
                                VMM_FREE_TYPE_RELEASE);
    if (SUCCEEDED(status))
    {
        LOG_ERROR("SyscallVirtualFree succeeded for memory which was not allocated through SyscallVirtualAlloc!\n");
        return status;
    }

    status = SyscallVirtualFree(IntrinAddressOfReturnAddress,
                                0,
                                0);
    if (SUCCEEDED(status))
    {
        LOG_ERROR("SyscallVirtualFree succeeded for memory which was not allocated through SyscallVirtualAlloc!\n");
        return status;
    }


    LOG_TEST_PASS;

    return STATUS_SUCCESS;
}