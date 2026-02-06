#include "common_lib.h"
#include "lock_common.h"
#include "cal_atomic.h"
#include "cal_assembly.h"
#include "cal_intrin.h"

#ifndef _COMMONLIB_NO_LOCKS_

typedef struct _MONITOR_LOCK_SYSTEM_DATA
{
    MONITOR_LOCK      Lock;
    LIST_ENTRY        MonitorLockList;
} MONITOR_LOCK_SYSTEM_DATA, *PMONITOR_LOCK_SYSTEM_DATA;

static MONITOR_LOCK_SYSTEM_DATA m_monitorLockData;

void
MonitorLockSystemInit(
    void
    )
{
    memzero(&m_monitorLockData, sizeof(MONITOR_LOCK_SYSTEM_DATA));
    InitializeListHead(&m_monitorLockData.MonitorLockList);
    AtomicExchange8(&m_monitorLockData.Lock.Lock.State, LOCK_FREE);
}

void
MonitorLockSystemGetMonitorLockList(
    OUT BOOLEAN*           IsSpinlock,
    OUT PMONITOR_LOCK*     ListLock,
    OUT PLIST_ENTRY*       ListHead   
    )
{
    ASSERT(IsSpinlock != NULL);
    ASSERT(ListLock != NULL);
    ASSERT(ListHead != NULL);

    *IsSpinlock = FALSE;
    *ListLock = &m_monitorLockData.Lock;
    *ListHead = &m_monitorLockData.MonitorLockList;
}

void
MonitorLockInit(
    OUT         PMONITOR_LOCK       Lock
    )
{
    INTR_STATE oldState;

    ASSERT(NULL != Lock);

    memzero(Lock, sizeof(MONITOR_LOCK));

    AtomicExchange8(&Lock->Lock.State, LOCK_FREE);
    
    MonitorLockAcquire(&m_monitorLockData.Lock, &oldState);
    InsertTailList(&m_monitorLockData.MonitorLockList, &Lock->Lock.AllList);
    MonitorLockRelease(&m_monitorLockData.Lock, oldState);
}

void
MonitorLockSetName(
    INOUT       PMONITOR_LOCK       Lock,
    IN          char*               Name
    )
{
    DWORD length;

    ASSERT(Lock != NULL);
    ASSERT(Name != NULL);

    length = strlen_s(Name, 16);
    if (length > 15)
    {
        length = 15;
        Lock->Lock.Name[15] = 0;
    }
    strncpy(Lock->Lock.Name, Name, length);
}

// DO NOT CHANGE THE NAME OF THIS FUNCTION
void
MonitorLockAcquire(
    INOUT       PMONITOR_LOCK       Lock,
    OUT         INTR_STATE*         IntrState
    )
{
    // See spinlock.c
    BOOLEAN spinning;
    PVOID pCurrentCpu;

    spinning = FALSE;

    ASSERT(NULL != Lock);
    ASSERT(NULL != IntrState);

    *IntrState = CpuIntrDisable();

    pCurrentCpu = CpuGetCurrent();

    ASSERT_INFO(pCurrentCpu != Lock->Lock.Holder,
                "Lock initial taken by function 0x%X, now called by 0x%X\n",
                Lock->Lock.FunctionWhichTookLock,
                *((PVOID*)IntrinAddressOfReturnAddress)
                );

// warning C4127: conditional expression is constant
MSVC_WARNING_SUPPRESS(4127)
    while(TRUE)
    {
        spinning = TRUE;

        AsmMonitor(Lock, 0, 0);

        if (LOCK_FREE == AtomicCompareExchange8(&Lock->Lock.State, LOCK_TAKEN, LOCK_FREE))
        {
            break;
        }

        AsmMwait(0, 0);
    }

    spinning = FALSE;

    ASSERT(NULL == Lock->Lock.FunctionWhichTookLock);
    ASSERT(NULL == Lock->Lock.Holder);

    Lock->Lock.Holder = pCurrentCpu;
    Lock->Lock.FunctionWhichTookLock = *((PVOID*)IntrinAddressOfReturnAddress);

    ASSERT(LOCK_TAKEN == Lock->Lock.State);
}

BOOL_SUCCESS
BOOLEAN
MonitorLockTryAcquire(
    INOUT       PMONITOR_LOCK       Lock,
    OUT         INTR_STATE*         IntrState
    )
{
    BOOLEAN acquired;

    PVOID pCurrentCpu;

    ASSERT(NULL != Lock);
    ASSERT(NULL != IntrState);

    *IntrState = CpuIntrDisable();

    pCurrentCpu = CpuGetCurrent();

    acquired = (LOCK_FREE == AtomicCompareExchange8(&Lock->Lock.State, LOCK_TAKEN, LOCK_FREE));
    if (!acquired)
    {
        CpuIntrSetState(*IntrState);
    }
    else
    {
        ASSERT(NULL == Lock->Lock.FunctionWhichTookLock);
        ASSERT(NULL == Lock->Lock.Holder);

        Lock->Lock.Holder = pCurrentCpu;
        Lock->Lock.FunctionWhichTookLock = *((PVOID*)IntrinAddressOfReturnAddress);

        ASSERT(LOCK_TAKEN == Lock->Lock.State);
    }

    return acquired;
}

BOOLEAN
MonitorLockIsOwner(
    IN          PMONITOR_LOCK       Lock
    )
{
    return CpuGetCurrent() == Lock->Lock.Holder;
}

void
MonitorLockRelease(
    INOUT       PMONITOR_LOCK       Lock,
    IN          INTR_STATE          OldIntrState
    )
{
    PVOID pCurrentCpu = CpuGetCurrent();

    ASSERT(NULL != Lock);
    ASSERT(pCurrentCpu == Lock->Lock.Holder);
    ASSERT(INTR_OFF == CpuIntrGetState());

    Lock->Lock.Holder = NULL;
    Lock->Lock.FunctionWhichTookLock = NULL;

    AtomicExchange8(&Lock->Lock.State, LOCK_FREE);

    CpuIntrSetState(OldIntrState);
}

void
MonitorLockDestroy(
    INOUT       PMONITOR_LOCK       Lock
    )
{
    INTR_STATE oldState;

    ASSERT(Lock != NULL);

    MonitorLockAcquire(&m_monitorLockData.Lock, &oldState);
    RemoveEntryList(&Lock->Lock.AllList);
    MonitorLockRelease(&m_monitorLockData.Lock, oldState);

    memzero(Lock, sizeof(MONITOR_LOCK));
}

#endif // _COMMONLIB_NO_LOCKS_
