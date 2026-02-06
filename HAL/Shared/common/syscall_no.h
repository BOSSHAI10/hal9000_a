#pragma once

typedef enum _SYSCALL_ID
{
    SyscallIdIdentifyVersion,

    // Thread Management
    SyscallIdThreadExit,
    SyscallIdThreadCreate,
    SyscallIdThreadGetTid,
    SyscallIdThreadWaitForTermination,
    SyscallIdThreadCloseHandle,
    SyscallIdGetThreadPriority,      // Adăugat pt proiect 
    SyscallIdSetThreadPriority,      // Adăugat pt proiect

    // Process Management
    SyscallIdProcessExit,
    SyscallIdProcessCreate,
    SyscallIdProcessGetPid,
    SyscallIdProcessWaitForTermination,
    SyscallIdProcessCloseHandle,
    SyscallIdProcessGetName,         // Adăugat pt proiect
    SyscallIdGetNumberOfThreadsForCurrentProcess, // Adăugat pt proiect

    // Memory management 
    SyscallIdVirtualAlloc,
    SyscallIdVirtualFree,

    // File management
    SyscallIdFileCreate,
    SyscallIdFileClose,
    SyscallIdFileRead,
    SyscallIdFileWrite,

    // System Info
    SyscallIdGetCurrentCPUID,        // Adăugat pt proiect
    SyscallIdGetCPUUtilization,      // Adăugat pt proiect


    SyscallIdGetPagePhysAddr,
    SyscallIdGetPageFaultNo,
    SyscallIdGetPageInternalFragmentation,

    SyscallIdReserved = SyscallIdFileWrite + 1
} SYSCALL_ID;
