#include "common_lib.h"
#include "lock_common.h"

#ifndef _COMMONLIB_NO_LOCKS_

PFUNC_LockSystemGetLockList LockSystemGetLockList = NULL;

PFUNC_LockInit           LockInit = NULL;

PFUNC_LockSetName        LockSetName = NULL;

PFUNC_LockAcquire        LockAcquire = NULL;

PFUNC_LockTryAcquire     LockTryAcquire = NULL;

PFUNC_LockRelease        LockRelease = NULL;

PFUNC_LockIsOwner        LockIsOwner = NULL;

PFUNC_LockDestroy        LockDestroy = NULL;

WARNING_PUSH
// warning C4028: formal parameter 1 different from declaration
// David Sipos: Modified to 4113, in VS2022 I get this error
MSVC_WARNING_DISABLE(4113) // Error for VS2022
MSVC_WARNING_DISABLE(4028) // Error for VS2019
GNU_WARNING_IGNORE("-Wincompatible-pointer-types")

void
LockSystemInit(
    IN      BOOLEAN             MonitorSupport
    )
{

    if (MonitorSupport)
    {
        // we have monitor support
        LockSystemGetLockList = MonitorLockSystemGetMonitorLockList;
        LockInit = MonitorLockInit;
        LockSetName = MonitorLockSetName;
        LockAcquire = MonitorLockAcquire;
        LockTryAcquire = MonitorLockTryAcquire;
        LockIsOwner = MonitorLockIsOwner;
        LockRelease = MonitorLockRelease;
        LockDestroy = MonitorLockDestroy;
        MonitorLockSystemInit();
    }
    else
    {
        // use classic spinlock
        LockSystemGetLockList = SpinlockSystemGetSpinlockList;
        LockInit = SpinlockInit;
        LockSetName = SpinlockSetName;
        LockAcquire = SpinlockAcquire;
        LockTryAcquire = SpinlockTryAcquire;
        LockIsOwner = SpinlockIsOwner;
        LockRelease = SpinlockRelease;
        LockDestroy = SpinlockDestroy;
        SpinlockSystemInit();
    }
}

WARNING_POP

#endif // _COMMONLIB_NO_LOCKS_
