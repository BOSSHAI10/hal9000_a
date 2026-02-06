#pragma once

#include"lock_common.h"
#include "mutex.h"
#include "rw_spinlock.h"

// DO NOT CHANGE ANYTHING IN THIS FILE.

STATUS
DebugDatabasePreinit(
    void
    );

void
DebugDatabaseSetMutexList(
    IN PLOCK             ListLock,
    IN PLIST_ENTRY       ListHead
    );

void
DebugDatabaseSetThreadList(
    IN PLOCK             ListLock,
    IN PLIST_ENTRY       ListHead
    );

void
DebugDatabaseSetProcessList(
    IN PMUTEX           ListMutex,
    IN PLIST_ENTRY      ListHead
    );

void
DebugDatabaseSetLockLists(
    IN BOOLEAN          IsSpinlock,
    IN PLOCK            LockLock,
    IN PLIST_ENTRY      LockkList,
    IN PRW_SPINLOCK     RwSpinlockLock,
    IN PLIST_ENTRY      RwSpinlockList
    );

void
DebugSystemSetFilesList(
    IN PLOCK            ListLock,
    IN PLIST_ENTRY      ListHead
    );

void
DebugSystemSetEventList(
    IN PLOCK            ListLock,
    IN PLIST_ENTRY      ListHead
    );
