#include "common_lib.h"
#include "lock_common.h"
#include "cal_atomic.h"
#include "cal_assembly.h"

#ifndef _COMMONLIB_NO_LOCKS_

typedef struct _RW_SPINLOCK_SYSTEM_DATA
{
    RW_SPINLOCK     Lock;
    LIST_ENTRY      RwSpinlockList;
} RW_SPINLOCK_SYSTEM_DATA, *PRW_SPINLOCK_SYSTEM_DATA;

static RW_SPINLOCK_SYSTEM_DATA m_rwSpinlockData;

void
RwSpinlockSystemInit(
    void
    )
{
    memzero(&m_rwSpinlockData, sizeof(RW_SPINLOCK_SYSTEM_DATA));
    InitializeListHead(&m_rwSpinlockData.RwSpinlockList);
}

void
RwSpinlockSystemGetLockList(
    OUT PRW_SPINLOCK*      ListLock,
    OUT PLIST_ENTRY*       ListHead
    )
{
    ASSERT(ListLock != NULL);
    ASSERT(ListHead != NULL);

    *ListLock = &m_rwSpinlockData.Lock;
    *ListHead = &m_rwSpinlockData.RwSpinlockList;
}

void
RwSpinlockInit(
    OUT     PRW_SPINLOCK    Spinlock
    )
{
    INTR_STATE oldState;

    ASSERT( NULL != Spinlock );

    memzero( Spinlock, sizeof(RW_SPINLOCK));

    RwSpinlockAcquireExclusive(&m_rwSpinlockData.Lock, &oldState);
    InsertTailList(&m_rwSpinlockData.RwSpinlockList, &Spinlock->AllList);
    RwSpinlockReleaseExclusive(&m_rwSpinlockData.Lock, oldState);
}

void
RwSpinlockSetName(
    INOUT   PRW_SPINLOCK    Spinlock,
    IN      char*           Name
    )
{
    DWORD length;

    ASSERT(Spinlock != NULL);
    ASSERT(Name != NULL);

    length = strlen_s(Name, 16);
    if (length > 15)
    {
        length = 15;
        Spinlock->Name[15] = 0;
    }
    strncpy(Spinlock->Name, Name, length);
}

// DO NOT CHANGE THE NAME OF THIS FUNCTION
REQUIRES_NOT_HELD_LOCK(*Spinlock)
_When_(Exclusive, ACQUIRES_EXCL_AND_NON_REENTRANT_LOCK(*Spinlock))
_When_(!Exclusive, ACQUIRES_SHARED_AND_NON_REENTRANT_LOCK(*Spinlock))
void
RwSpinlockAcquire(
    INOUT   RW_SPINLOCK     *Spinlock,
    OUT     INTR_STATE*     IntrState,
    IN      BOOLEAN         Exclusive
    )
{
    BOOLEAN spinning;

    // See spinlock.c
    spinning = FALSE;

    ASSERT( NULL != Spinlock );
    ASSERT( NULL != IntrState );

    *IntrState = CpuIntrDisable();

    if (Exclusive)
    {
        AtomicIncrement16(&Spinlock->WaitingWriters);

        // because this is done on DWORD it will affect ActiveWrite and ActiveReaders
        while (0 != AtomicCompareExchange32((volatile DWORD*) &Spinlock->ActiveWriter, 1, 0))
        {
            spinning = TRUE;
            AsmPause();
        }

        spinning = FALSE;

        // we're here => we're the active writer
        // => we're no longer a waiting writer
        AtomicDecrement16(&Spinlock->WaitingWriters);
        _Analysis_assume_lock_acquired_(*Spinlock);
    }
    else
    {
        DWORD pseudoActiveWriter = MAX_WORD + 1;

        // shared

        // pretend to take lock exclusively
        // but instead of checking ActiveWriter and ActiveReaders
        // check WaitingWriters and ActiveWriter (so writers will have priority)
        while (0 != AtomicCompareExchange32((volatile DWORD*) &Spinlock->WaitingWriters, pseudoActiveWriter, 0))
        {
            spinning = TRUE;
            AsmPause();
        }

        spinning = FALSE;

        // we're here => we're an active reader
        AtomicIncrement16(&Spinlock->ActiveReaders);

        ASSERT( 1 == Spinlock->ActiveWriter );

        // let's be honest now and tell the world
        // we're not a writer
        AtomicDecrement16(&Spinlock->ActiveWriter);
        _Analysis_assume_lock_acquired_(*Spinlock);
    }
}

_When_(Exclusive, REQUIRES_EXCL_LOCK(*Spinlock) RELEASES_EXCL_AND_NON_REENTRANT_LOCK(*Spinlock))
_When_(!Exclusive, REQUIRES_SHARED_LOCK(*Spinlock) RELEASES_SHARED_AND_NON_REENTRANT_LOCK(*Spinlock))
void
RwSpinlockRelease(
    INOUT   RW_SPINLOCK     *Spinlock,
    IN      INTR_STATE      IntrState,
    IN      BOOLEAN         Exclusive
    )
{
    ASSERT( NULL != Spinlock );

    if (Exclusive)
    {
        ASSERT( 1 == Spinlock->ActiveWriter );

        AtomicDecrement16(&Spinlock->ActiveWriter);
        _Analysis_assume_lock_released_(*Spinlock);
    }
    else
    {
        // shared

        // in this case may have 1 pseudo-active writer as well until
        // the reader 'confesses' to the world it is not a writer
        AtomicDecrement16(&Spinlock->ActiveReaders);
        _Analysis_assume_lock_released_(*Spinlock);
    }

    CpuIntrSetState(IntrState);
}

void
RwSpinlockDestroy(
    INOUT   RW_SPINLOCK     *Spinlock
    )
{
    INTR_STATE oldState;

    ASSERT(Spinlock != NULL);

    RwSpinlockAcquireExclusive(&m_rwSpinlockData.Lock, &oldState);
    RemoveEntryList(&Spinlock->AllList);
    RwSpinlockReleaseExclusive(&m_rwSpinlockData.Lock, oldState);
    
    memzero(Spinlock, sizeof(RW_SPINLOCK));
}

#endif // _COMMONLIB_NO_LOCKS_
