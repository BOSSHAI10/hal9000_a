#pragma once

C_HEADER_START

#ifndef _COMMONLIB_NO_LOCKS_
#include "list.h"
#include "spinlock.h"
#include "monlock.h"
#include "rw_spinlock.h"
#include "rec_rw_spinlock.h"
#include "cal_annotate.h"

typedef
INTR_STATE
(CDECL FUNC_IntrDisable)(
    void
    );

typedef
INTR_STATE
(CDECL FUNC_IntrEnable)(
    void
    );

typedef
INTR_STATE
(CDECL FUNC_IntrGetState)(
    void
    );

typedef
INTR_STATE
(CDECL FUNC_IntrSetState)(
    IN      INTR_STATE      State
    );

typedef
PVOID
(CDECL FUNC_CpuGetCurrent)(
    void
    );

extern FUNC_CpuGetCurrent   CpuGetCurrent;
extern FUNC_IntrDisable     CpuIntrDisable;
extern FUNC_IntrEnable      CpuIntrEnable;
extern FUNC_IntrGetState    CpuIntrGetState;
extern FUNC_IntrSetState    CpuIntrSetState;

#define LOCK_TAKEN          1
#define LOCK_FREE           0

typedef union _LOCK
{
    SPINLOCK        SpinLock;
    MONITOR_LOCK    MonitorLock;
} LOCK, *PLOCK;

typedef
void
(CDECL FUNC_LockSystemGetLockList)(
    OUT BOOLEAN*           IsSpinlock,
    OUT PLOCK*             ListLock,
    OUT PLIST_ENTRY*       ListHead
    );

typedef FUNC_LockSystemGetLockList* PFUNC_LockSystemGetLockList;

typedef
void
(CDECL FUNC_LockInit)(
    OUT         PLOCK           Lock
    );

typedef FUNC_LockInit*          PFUNC_LockInit;

typedef
void
(CDECL FUNC_LockSetName)(
    INOUT PLOCK            Lock,
    IN    char*            Name
    );

typedef FUNC_LockSetName*       PFUNC_LockSetName;

typedef
void
REQUIRES_NOT_HELD_LOCK(*Lock)
ACQUIRES_EXCL_AND_NON_REENTRANT_LOCK(*Lock)
(CDECL FUNC_LockAcquire)(
    INOUT       PLOCK           Lock,
    OUT         INTR_STATE*     IntrState
    );

typedef FUNC_LockAcquire*       PFUNC_LockAcquire;

typedef
BOOLEAN
(CDECL FUNC_LockTryAcquire)(
    INOUT       PLOCK           Lock,
    OUT         INTR_STATE*     IntrState
    );

typedef FUNC_LockTryAcquire*    PFUNC_LockTryAcquire;

typedef
BOOLEAN
(CDECL FUNC_LockIsOwner)(
    IN          PLOCK           Lock
    );

typedef FUNC_LockIsOwner*       PFUNC_LockIsOwner;

typedef
void
REQUIRES_EXCL_LOCK(*Lock)
RELEASES_EXCL_AND_NON_REENTRANT_LOCK(*Lock)
(CDECL FUNC_LockRelease)(
    INOUT       PLOCK           Lock,
    IN          INTR_STATE      OldIntrState
    );

typedef FUNC_LockRelease*       PFUNC_LockRelease;

typedef
void
(CDECL FUNC_LockDestroy)(
    INOUT       PLOCK           Lock       
    );

typedef FUNC_LockDestroy*       PFUNC_LockDestroy;

extern PFUNC_LockSystemGetLockList LockSystemGetLockList;

extern PFUNC_LockInit           LockInit;

extern PFUNC_LockSetName        LockSetName;

extern PFUNC_LockAcquire        LockAcquire;

extern PFUNC_LockTryAcquire     LockTryAcquire;

extern PFUNC_LockRelease        LockRelease;

extern PFUNC_LockIsOwner        LockIsOwner;

extern PFUNC_LockDestroy        LockDestroy;

void
LockSystemInit(
    IN      BOOLEAN             MonitorSupport
    );
#endif // _COMMONLIB_NO_LOCKS_
C_HEADER_END
