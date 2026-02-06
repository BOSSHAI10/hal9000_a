#include "common_lib.h"
#include "lock_common.h"
#include "cal_atomic.h"
#include "cal_assembly.h"
#include "cal_intrin.h"

#ifndef _COMMONLIB_NO_LOCKS_

typedef struct _SPINLOCK_SYSTEM_DATA
{
    SPINLOCK           Lock;
    LIST_ENTRY         SpinlockList; 
} SPINLOCK_SYSTEM_DATA, *PSPINLOCK_SYSTEM_DATA;

static SPINLOCK_SYSTEM_DATA m_spinlockData;

void
SpinlockSystemInit(
    void
    )
{
    memzero(&m_spinlockData, sizeof(SPINLOCK_SYSTEM_DATA));
    InitializeListHead(&m_spinlockData.SpinlockList);
    AtomicExchange8(&m_spinlockData.Lock.State, LOCK_FREE);
}

void
SpinlockSystemGetSpinlockList(
    OUT BOOLEAN*           IsSpinlock,
    OUT PSPINLOCK*         ListLock,
    OUT PLIST_ENTRY*       ListHead   
    )
{
    ASSERT(IsSpinlock != NULL);
    ASSERT(ListLock != NULL);
    ASSERT(ListHead != NULL);

    *IsSpinlock = TRUE;
    *ListLock = &m_spinlockData.Lock;
    *ListHead = &m_spinlockData.SpinlockList;
}

void
SpinlockInit(
    OUT         PSPINLOCK       Lock
    )
{
    INTR_STATE oldState;

    ASSERT(NULL != Lock);

    memzero(Lock, sizeof(SPINLOCK));

    AtomicExchange8(&Lock->State, LOCK_FREE);

    SpinlockAcquire(&m_spinlockData.Lock, &oldState);
    InsertTailList(&m_spinlockData.SpinlockList, &Lock->AllList);
    SpinlockRelease(&m_spinlockData.Lock, oldState);
}

void
SpinlockSetName(
    INOUT       PSPINLOCK       Lock,
    IN          char*           Name
    )
{
    DWORD length;

    ASSERT(Lock != NULL);
    ASSERT(Name != NULL);

    length = strlen_s(Name, 16);
    if (length > 15)
    {
        length = 15;
        Lock->Name[15] = 0;
    }
    strncpy(Lock->Name, Name, length);
}

// DO NOT CHANGE THE NAME OF THIS FUNCTION
void
SpinlockAcquire(
    INOUT       PSPINLOCK       Lock,
    OUT         INTR_STATE*     IntrState
    )
{
    // Read by the debugger to see if
    // the core is waiting.
    // I think this is the simplest way to do it,
    // without relying on this specific implementation.
    // Also it makes detection uniform across all lock types.
    BOOLEAN spinning;
    PVOID pCurrentCpu;
    
    spinning = FALSE;

    ASSERT(NULL != Lock);
    ASSERT(NULL != IntrState);

    *IntrState = CpuIntrDisable();

    pCurrentCpu = CpuGetCurrent();

    ASSERT_INFO(pCurrentCpu != Lock->Holder,
                "Lock initial taken by function 0x%X, now called by 0x%X\n",
                Lock->FunctionWhichTookLock,
                *((PVOID*)IntrinAddressOfReturnAddress)
                );

    while (LOCK_TAKEN == AtomicCompareExchange8(&Lock->State, LOCK_TAKEN, LOCK_FREE))
    {
        spinning = TRUE;
        AsmPause();
    }

    spinning = FALSE;

    ASSERT(NULL == Lock->FunctionWhichTookLock);
    ASSERT(NULL == Lock->Holder);

    Lock->Holder = pCurrentCpu;
    Lock->FunctionWhichTookLock = *( (PVOID*) IntrinAddressOfReturnAddress );

    ASSERT(LOCK_TAKEN == Lock->State);
}

BOOL_SUCCESS
BOOLEAN
SpinlockTryAcquire(
    INOUT       PSPINLOCK       Lock,
    OUT         INTR_STATE*     IntrState
    )
{
    PVOID pCurrentCpu;

    BOOLEAN acquired;

    ASSERT(NULL != Lock);
    ASSERT(NULL != IntrState);

    *IntrState = CpuIntrDisable();

    pCurrentCpu = CpuGetCurrent();

    acquired = (LOCK_FREE == AtomicCompareExchange8(&Lock->State, LOCK_TAKEN, LOCK_FREE));
    if (!acquired)
    {
        CpuIntrSetState(*IntrState);
    }
    else
    {
        ASSERT(NULL == Lock->FunctionWhichTookLock);
        ASSERT(NULL == Lock->Holder);

        Lock->Holder = pCurrentCpu;
        Lock->FunctionWhichTookLock = *((PVOID*)IntrinAddressOfReturnAddress);

        ASSERT(LOCK_TAKEN == Lock->State);
    }

    return acquired;
}

BOOLEAN
SpinlockIsOwner(
    IN          PSPINLOCK       Lock
    )
{
    return CpuGetCurrent() == Lock->Holder;
}

void
SpinlockRelease(
    INOUT       PSPINLOCK       Lock,
    IN          INTR_STATE      OldIntrState
    )
{
    PVOID pCurrentCpu = CpuGetCurrent();

    ASSERT(NULL != Lock);
    ASSERT_INFO(pCurrentCpu == Lock->Holder,
                "LockTaken by CPU: 0x%X in function: 0x%X\nNow release by CPU: 0x%X in function: 0x%X\n",
                Lock->Holder, Lock->FunctionWhichTookLock,
                pCurrentCpu, *( (PVOID*) IntrinAddressOfReturnAddress) );
    ASSERT(INTR_OFF == CpuIntrGetState());

    Lock->Holder = NULL;
    Lock->FunctionWhichTookLock = NULL;

    AtomicExchange8(&Lock->State, LOCK_FREE);

    CpuIntrSetState(OldIntrState);
}

void
SpinlockDestroy(
    INOUT       PSPINLOCK       Lock
    )
{
    INTR_STATE oldState;

    ASSERT(Lock != NULL);

    SpinlockAcquire(&m_spinlockData.Lock, &oldState);
    RemoveEntryList(&Lock->AllList);
    SpinlockRelease(&m_spinlockData.Lock, oldState);

    memzero(Lock, sizeof(SPINLOCK));
}

#endif // _COMMONLIB_NO_LOCKS_
