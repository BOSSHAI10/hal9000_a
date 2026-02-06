#pragma once

#include "list.h"
#include "lock_common.h"

typedef enum _EX_EVT_TYPE
{
    ExEventTypeNotification,        // notifies all threads
    ExEventTypeSynchronization,     // notifies only one thread

    ExEventTypeReserved
} EX_EVT_TYPE;

typedef struct _EX_EVENT
{
    // DO NOT CHANGE THE NAMES OF THESE FIELDS
    LOCK                EventLock;
    LIST_ENTRY          WaitingList;
    EX_EVT_TYPE         EventType;
    volatile BYTE       Signaled;
    LIST_ENTRY          AllList;
    char                Name[16];
} EX_EVENT, *PEX_EVENT;

//******************************************************************************
// Function:     ExEventSystemPreinit
// Description:  Initializes the executive event system.
// Returns:      STATUS
// Parameter:    void
//******************************************************************************
STATUS
ExEventSystemPreinit(
    void
    );

//******************************************************************************
// Function:     ExEventSystemGetEventList
// Description:  Retrieves the global executive event list and its lock.
// Returns:      void
// Parameter:    OUT PLOCK* ListLock
// Parameter:    OUT PLIST_ENTRY* ListHead
//******************************************************************************
void
ExEventSystemGetEventList(
    OUT PLOCK*             ListLock,
    OUT PLIST_ENTRY*       ListHead
    );

//******************************************************************************
// Function:     ExEventInit
// Description:  Initializes an executive event. As in the case of primitive
//               events, these may be notification or synchronization events.
//               It notifies the debugger.
// Returns:      STATUS
// Parameter:    OUT EX_EVENT * Event
// Parameter:    IN EX_EVT_TYPE EventType
// Parameter:    IN BOOLEAN Signaled
//******************************************************************************
STATUS
ExEventInit(
    OUT     EX_EVENT*     Event,
    IN      EX_EVT_TYPE   EventType,
    IN      BOOLEAN       Signaled
    );

//******************************************************************************
// Function:     ExEventSetName
// Description:  Sets the name of an executive event.
// Returns:      void
// Parameter:    OUT PMUTEX Mutex
// Parameter:    IN char* Name - name of the mutex, maximum length 15 chars,
//               excluding the null terminator.
// NOTE:         It is not mandatory to call this function. The name helps
//               helps to identify it in the debugger.
//******************************************************************************
_No_competing_thread_
void
ExEventSetName(
    INOUT       PEX_EVENT   Event,
    IN          char*       Name
    );

//******************************************************************************
// Function:     ExEventSignal
// Description:  Signals an event. If the waiting list is not empty it will
//               wakeup one or multiple threads depending on the event type.
// Returns:      void
// Parameter:    INOUT EX_EVENT * Event
//******************************************************************************
void
ExEventSignal(
    INOUT   EX_EVENT*      Event
    );

//******************************************************************************
// Function:     ExEventClearSignal
// Description:  Clears an event signal.
// Returns:      void
// Parameter:    INOUT EX_EVENT * Event
//******************************************************************************
void
ExEventClearSignal(
    INOUT   EX_EVENT*      Event
    );

//******************************************************************************
// Function:     ExEventWaitForSignal
// Description:  Waits for an event to be signaled. If the event is not signaled
//               the calling thread will be placed in a waiting list and its
//               execution will be blocked.
// Returns:      void
// Parameter:    INOUT EX_EVENT * Event
//******************************************************************************
void
ExEventWaitForSignal(
    INOUT   EX_EVENT*      Event
    );

//******************************************************************************
// Function:     ExEventDestroy
// Description:  Destroys an executive event. It should be called
//               to notify the debugger.
// Returns:      void
// Parameter:    INOUT EX_EVENT * Event
//******************************************************************************
void
ExEventDestroy(
    INOUT     EX_EVENT*     Event
    );
