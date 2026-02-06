#include "common_lib.h"
#include "event.h"
#include "cal_atomic.h"
#include "cal_assembly.h"

#define EVENT_STATE_NOT_SIGNALED        0
#define EVENT_STATE_SIGNALED            1

STATUS
EvtInitialize(
    OUT     EVENT*          Event,
    IN      EVENT_TYPE      EventType,
    IN      BOOLEAN         Signaled
    )
{
    BYTE eventState;

    if (NULL == Event)
    {
        return STATUS_INVALID_PARAMETER1;
    }

    if (EventType >= EventTypeReserved)
    {
        return STATUS_INVALID_PARAMETER2;
    }

    eventState = Signaled ? EVENT_STATE_SIGNALED : EVENT_STATE_NOT_SIGNALED;

    Event->EventType = EventType;

    AtomicExchange8(&Event->State, eventState);

    return STATUS_SUCCESS;
}

void
EvtSignal(
    INOUT   EVENT*          Event
    )
{
    ASSERT(NULL != Event);

    AtomicExchange8(&Event->State, EVENT_STATE_SIGNALED);
}

void
EvtClearSignal(
    INOUT   EVENT*          Event
    )
{
    ASSERT(NULL != Event);

    AtomicExchange8(&Event->State, EVENT_STATE_NOT_SIGNALED);
}

void
EvtWaitForSignal(
    INOUT   EVENT*          Event
    )
{
    BYTE exchangeValue;

    ASSERT(NULL != Event);

    exchangeValue = EventTypeNotification == Event->EventType ? 
                        EVENT_STATE_SIGNALED : EVENT_STATE_NOT_SIGNALED;

    // wait for the event to be signaled
    while (EVENT_STATE_SIGNALED != (DWORD)AtomicCompareExchange8(&Event->State, exchangeValue, EVENT_STATE_SIGNALED))
    {
        AsmPause();
    }
}

BOOLEAN
EvtIsSignaled(
    INOUT   EVENT*          Event
    )
{
    BYTE exchangeValue;
    BYTE initialValue;

    ASSERT( NULL != Event );

    exchangeValue = EventTypeNotification == Event->EventType ? 
                        EVENT_STATE_SIGNALED : EVENT_STATE_NOT_SIGNALED;

    initialValue = AtomicCompareExchange8(&Event->State, exchangeValue, EVENT_STATE_SIGNALED);

    return (initialValue == EVENT_STATE_SIGNALED);
}