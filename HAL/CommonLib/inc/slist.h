#pragma once

C_HEADER_START

#include "cal_annotate.h"

typedef struct _CL_SLIST_ENTRY
{
    struct _CL_SLIST_ENTRY*    Next;
} CL_SLIST_ENTRY, *PCL_SLIST_ENTRY;

ALWAYS_INLINE
void
ClInitializeSListHead(
    OUT     PCL_SLIST_ENTRY ListHead
    )
{
    ListHead->Next = NULL;
}

ALWAYS_INLINE
PCL_SLIST_ENTRY
ClPopEntryList(
    INOUT PCL_SLIST_ENTRY ListHead
)
{
    PCL_SLIST_ENTRY FirstEntry;

    FirstEntry = ListHead->Next;
    if (FirstEntry != NULL)
    {
        ListHead->Next = FirstEntry->Next;
    }

    return FirstEntry;
}

ALWAYS_INLINE
void
ClPushEntryList(
    INOUT PCL_SLIST_ENTRY ListHead,
    INOUT PCL_SLIST_ENTRY Entry
)
{
    Entry->Next = ListHead->Next;
    ListHead->Next = Entry;
    return;
}
C_HEADER_END
