#pragma once

C_HEADER_START
#pragma pack(push,16)
typedef struct _SPINLOCK
{
    // DO NOT CHANGE THE NAMES OF THESE FIELDS
    volatile BYTE       State;
    PVOID               Holder;
    PVOID               FunctionWhichTookLock;
    char                Name[16];
    LIST_ENTRY          AllList;
} SPINLOCK, *PSPINLOCK;
#pragma pack(pop)

//******************************************************************************
// Function:     SpinlockSystemInit
// Description:  Initializes the spinlock system.
// Returns:      void
// Parameter:    void
//******************************************************************************
void
SpinlockSystemInit(
    void
    );

//******************************************************************************
// Function:     SpinlockSystemGetSpinlockList
// Description:  Retrieves the global spinlock list.
// Returns:      void
// Parameter:    OUT BOOLEAN* IsSpinlock
// Parameter:    OUT PLOCK*   ListLock
// Parameter:    OUT PLIST_ENTRY* ListHead
//******************************************************************************
void
SpinlockSystemGetSpinlockList(
    OUT BOOLEAN*           IsSpinlock,
    OUT PSPINLOCK*         ListLock,
    OUT PLIST_ENTRY*       ListHead   
    );

//******************************************************************************
// Function:     SpinlockInit
// Description:  Initializes a spinlock. No other spinlock* function can be used
//               before this function is called.
// Returns:      void
// Parameter:    OUT PSPINLOCK Lock
//******************************************************************************
void
SpinlockInit(
    OUT         PSPINLOCK       Lock
    );

//******************************************************************************
// Function:     SpinlockSetName
// Description:  Sets the name of a spinlock.
// Returns:      void
// Parameter:    INOUT PSPINLOCK Lock
// Parameter:    IN char* Name - name of the spinlock, maximum length 15 chars,
//               excluding the null terminator.
// NOTE:         It is not mandatory to call this function. The name helps
//               helps to identify it in the debugger.
//               This function is not synchronized, you are responsible
//               to make sure it is called correctly.
//******************************************************************************
void
SpinlockSetName(
    INOUT       PSPINLOCK       Lock,
    IN          char*           Name
    );

//******************************************************************************
// Function:     SpinlockAcquire
// Description:  Spins until the Lock is acquired. On return interrupts will be
//               disabled and IntrState will hold the previous interruptibility
//               state.
// Returns:      void
// Parameter:    INOUT PSPINLOCK Lock
// Parameter:    OUT INTR_STATE * IntrState
//******************************************************************************
void
SpinlockAcquire(
    INOUT       PSPINLOCK       Lock,
    OUT         INTR_STATE*     IntrState
    );

//******************************************************************************
// Function:     SpinlockTryAcquire
// Description:  Attempts to acquire the Lock. If it is free then the function
//               will take the lock and return with the interrupts disabled and
//               IntrState will hold the previous interruptibility state.
// Returns:      BOOLEAN - TRUE if the lock was acquired, FALSE otherwise
// Parameter:    INOUT PSPINLOCK Lock
// Parameter:    OUT INTR_STATE * IntrState
//******************************************************************************
BOOL_SUCCESS
BOOLEAN
SpinlockTryAcquire(
    INOUT       PSPINLOCK       Lock,
    OUT         INTR_STATE*     IntrState
    );

//******************************************************************************
// Function:     SpinlockIsOwner
// Description:  Checks if the current CPU is the lock owner.
// Returns:      BOOLEAN
// Parameter:    IN PSPINLOCK Lock
//******************************************************************************
BOOLEAN
SpinlockIsOwner(
    IN          PSPINLOCK       Lock
    );

//******************************************************************************
// Function:     SpinlockRelease
// Description:  Releases a previously acquired Lock. OldIntrState should hold
//               the value previous returned by SpinlockAcquire or
//               SpinlockTryAcquire.
// Returns:      void
// Parameter:    INOUT PSPINLOCK Lock
// Parameter:    IN INTR_STATE OldIntrState
//******************************************************************************
void
SpinlockRelease(
    INOUT       PSPINLOCK       Lock,
    IN          INTR_STATE      OldIntrState
    );

//******************************************************************************
// Function:     SpinlockDestroy
// Description:  Destroys a spinlock.
// Returns:      void
// Parameter:    INOUT PSPINLOCK Lock
//******************************************************************************
void
SpinlockDestroy(
    INOUT       PSPINLOCK       Lock
    );

C_HEADER_END
