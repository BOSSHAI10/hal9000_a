#include "HAL9000.h"
#include "lock_common.h"
#include "debug_database.h"
#include "mutex.h"

// DO NOT CHANGE ANYTHING IN THIS FILE.

typedef struct _DEBUG_SYSTEM_DATA
{
    PLOCK               MutexLock;
    PLIST_ENTRY         MutexList;

    PLOCK               ThreadLock;
    PLIST_ENTRY         ThreadList;

    PMUTEX              ProcessMutex;
    PLIST_ENTRY         ProcessList;
    
    BOOLEAN             IsSpinlock;
    PLOCK               LockLock;
    PLIST_ENTRY         LockList;

    PRW_SPINLOCK       RwSpinlockLock;
    PLIST_ENTRY        RwSpinlockList;

    PLOCK              FileLock;
    PLIST_ENTRY        FileList;

    PLOCK              ExEventLock;
    PLIST_ENTRY        ExEventList;
} DEBUG_SYSTEM_DATA, *PDEBUG_SYSTEM_DATA;

static DEBUG_SYSTEM_DATA m_debugSystemData;

STATUS
DebugDatabasePreinit(
    void
    )
{
    memzero(&m_debugSystemData, sizeof(DEBUG_SYSTEM_DATA));    

    return STATUS_SUCCESS;
}

void
DebugDatabaseSetMutexList(
    IN PLOCK             ListLock,
    IN PLIST_ENTRY       ListHead
    )
{
    ASSERT(ListLock != NULL);
    ASSERT(ListHead != NULL);

    m_debugSystemData.MutexLock = ListLock;
    m_debugSystemData.MutexList = ListHead;
}

void
DebugDatabaseSetThreadList(
    IN PLOCK             ListLock,
    IN PLIST_ENTRY       ListHead
    )
{
    ASSERT(ListLock != NULL);
    ASSERT(ListHead != NULL);

    m_debugSystemData.ThreadLock = ListLock;
    m_debugSystemData.ThreadList = ListHead;
}

void
DebugDatabaseSetProcessList(
    IN PMUTEX           ListMutex,
    IN PLIST_ENTRY      ListHead
    )
{
    ASSERT(ListMutex != NULL);
    ASSERT(ListHead != NULL);

    m_debugSystemData.ProcessMutex = ListMutex;
    m_debugSystemData.ProcessList = ListHead;
}

void
DebugDatabaseSetLockLists(
    IN BOOLEAN          IsSpinlock,
    IN PLOCK            LockLock,
    IN PLIST_ENTRY      LockList,
    IN PRW_SPINLOCK     RwSpinlockLock,
    IN PLIST_ENTRY      RwSpinlockList
    )
{
    m_debugSystemData.IsSpinlock = IsSpinlock;
    m_debugSystemData.LockLock = LockLock;
    m_debugSystemData.LockList = LockList;
    m_debugSystemData.RwSpinlockLock = RwSpinlockLock;
    m_debugSystemData.RwSpinlockList = RwSpinlockList;
}

void
DebugSystemSetFilesList(
    IN PLOCK            ListLock,
    IN PLIST_ENTRY      ListHead
    )
{
    m_debugSystemData.FileLock = ListLock;
    m_debugSystemData.FileList = ListHead;
}

void
DebugSystemSetEventList(
    IN PLOCK            ListLock,
    IN PLIST_ENTRY      ListHead
    )
{
    m_debugSystemData.ExEventLock = ListLock;
    m_debugSystemData.ExEventList = ListHead;
}
