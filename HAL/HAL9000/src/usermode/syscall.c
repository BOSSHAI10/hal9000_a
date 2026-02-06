#include "HAL9000.h"
#include "syscall.h"
#include "syscall_defs.h"
#include "syscall_func.h"
#include "syscall_no.h"
#include "thread.h"
#include "thread_internal.h"
#include "cpumu.h"
#include "smp.h"
#include "mmu.h"
#include "process_internal.h"
#include "dmp_cpu.h"
#include "gdtmu.h"
#include "thread.h"
#include "cal_annotate.h"
#include "cal_assembly.h"
#include "cal_seh.h"
#include "status.h"
#include "vmm.h"

STATUS SyscallThreadGetTid(IN_OPT UM_HANDLE ThreadHandle,OUT TID* ThreadId);
STATUS SyscallProcessGetName(OUT char* ProcessName, IN QWORD ProcessNameMaxLen);
STATUS SyscallGetThreadPriority(OUT BYTE* ThreadPriority);
STATUS SyscallSetThreadPriority(IN BYTE ThreadPriority);
STATUS SyscallGetCurrentCPUID(OUT BYTE* CpuId);
STATUS SyscallGetNumberOfThreadsForCurrentProcess(OUT QWORD* ThreadNo);
STATUS SyscallGetCPUUtilization(IN_OPT BYTE* CpuId, OUT BYTE* Utilization);


extern void MS_ABI SyscallEntry();

#define SYSCALL_IF_VERSION_KM       SYSCALL_IMPLEMENTED_IF_VERSION

void
MS_ABI
SyscallHandler(
    INOUT   COMPLETE_PROCESSOR_STATE    *CompleteProcessorState
    )
{
    SYSCALL_ID sysCallId;
    PQWORD pSyscallParameters;
    PQWORD pParameters;
    STATUS status;
    REGISTER_AREA* usermodeProcessorState;

    ASSERT(CompleteProcessorState != NULL);

    // It is NOT ok to setup the FMASK so that interrupts will be enabled when the system call occurs
    // The issue is that we'll have a user-mode stack and we wouldn't want to receive an interrupt on
    // that stack. This is why we only enable interrupts here.
    ASSERT(CpuIntrGetState() == INTR_OFF);
    CpuIntrSetState(INTR_ON);

    LOG_TRACE_USERMODE("The syscall handler has been called!\n");

    status = STATUS_SUCCESS;
    pSyscallParameters = NULL;
    pParameters = NULL;
    usermodeProcessorState = &CompleteProcessorState->RegisterArea;

    __try
    {
        if (LogIsComponentTraced(LogComponentUserMode))
        {
            DumpProcessorState(CompleteProcessorState);
        }

        // Check if indeed the shadow stack is valid (the shadow stack is mandatory)
        pParameters = (PQWORD)usermodeProcessorState->RegisterValues[RegisterRbp];
        status = MmuIsBufferValid(pParameters, SHADOW_STACK_SIZE, PAGE_RIGHTS_READ, GetCurrentProcess());
        if (!SUCCEEDED(status))
        {
            LOG_FUNC_ERROR("MmuIsBufferValid", status);
            __leave;
        }

        sysCallId = usermodeProcessorState->RegisterValues[RegisterR8];

        LOG_TRACE_USERMODE("System call ID is %u\n", sysCallId);

        // The first parameter is the system call ID, we don't care about it => +1
        pSyscallParameters = (PQWORD)usermodeProcessorState->RegisterValues[RegisterRbp] + 1;

        // Dispatch syscalls
        switch (sysCallId)
        {
        case SyscallIdIdentifyVersion:
            status = SyscallValidateInterface((SYSCALL_IF_VERSION)*pSyscallParameters);
            break;
          case SyscallIdProcessExit:
            status = SyscallProcessExit((STATUS)*pSyscallParameters);
            break;
        case SyscallIdThreadExit:
            status = SyscallThreadExit((STATUS)*pSyscallParameters);
            break;
        case SyscallIdFileWrite:
            status = SyscallFileWrite(
                (UM_HANDLE)pSyscallParameters[0],
                (PVOID)pSyscallParameters[1],
                (QWORD)pSyscallParameters[2],
                (QWORD*)pSyscallParameters[3]
            );
            break;
        // STUDENT TODO: implement the rest of the syscalls 

        case SyscallIdThreadGetTid:
            status = SyscallThreadGetTid(
                (UM_HANDLE)pSyscallParameters[0], // ThreadHandle
                (TID*)pSyscallParameters[1]      // ThreadId
            );
            break;

        case SyscallIdProcessGetName:
            status = SyscallProcessGetName(
                (char*)pSyscallParameters[0],
                (QWORD)pSyscallParameters[1]
            );
            break;

        case SyscallIdGetThreadPriority:
            status = SyscallGetThreadPriority((BYTE*)pSyscallParameters[0]);
            break;

        case SyscallIdSetThreadPriority:
            status = SyscallSetThreadPriority((BYTE)pSyscallParameters[0]);
            break;

        case SyscallIdGetCurrentCPUID:
            status = SyscallGetCurrentCPUID((BYTE*)pSyscallParameters[0]);
            break;

        case SyscallIdGetNumberOfThreadsForCurrentProcess:
            status = SyscallGetNumberOfThreadsForCurrentProcess((QWORD*)pSyscallParameters[0]);
            break;

        case SyscallIdGetCPUUtilization:
            status = SyscallGetCPUUtilization(
                (BYTE*)pSyscallParameters[0],
                (BYTE*)pSyscallParameters[1]
            );
            break;

        case SyscallIdVirtualAlloc:
            status = SyscallVirtualAlloc(
                (PVOID)pSyscallParameters[0],
                (QWORD)pSyscallParameters[1],
                (VMM_ALLOC_TYPE)pSyscallParameters[2],
                (PAGE_RIGHTS)pSyscallParameters[3],
                (UM_HANDLE)pSyscallParameters[4],
                (QWORD)pSyscallParameters[5],
                (PVOID*)pSyscallParameters[6]
            );
            break;

        case SyscallIdVirtualFree:
            status = SyscallVirtualFree(
                (PVOID)pSyscallParameters[0],
                (QWORD)pSyscallParameters[1],
                (VMM_FREE_TYPE)pSyscallParameters[2]
            );
            break;

        case SyscallIdGetPageFaultNo:
            status = SyscallGetPageFaultNo(
                (PVOID)pSyscallParameters[0],
                (QWORD*)pSyscallParameters[1]
            );
            break;

        case SyscallIdGetPagePhysAddr:
            status = SyscallGetPagePhysAddr(
                (PVOID)pSyscallParameters[0],
                (PVOID*)pSyscallParameters[1]
            );
            break;

        case SyscallIdGetPageInternalFragmentation:
            status = SyscallGetPageInternalFragmentation(
                (PVOID)pSyscallParameters[0],
                (QWORD*)pSyscallParameters[1]
            );
            break;

        default:
            LOG_ERROR("Unimplemented syscall called from User-space!\n");
            status = STATUS_UNSUPPORTED;
            break;
        }

    }
    __finally
    {
        LOG_TRACE_USERMODE("Will set UM RAX to 0x%x\n", status);

        usermodeProcessorState->RegisterValues[RegisterRax] = status;

        CpuIntrSetState(INTR_OFF);
    }
}

void
SyscallPreinitSystem(
    void
    )
{

}

STATUS
SyscallInitSystem(
    void
    )
{
    return STATUS_SUCCESS;
}

STATUS
SyscallUninitSystem(
    void
    )
{
    return STATUS_SUCCESS;
}

void
SyscallCpuInit(
    void
    )
{
    IA32_STAR_MSR_DATA starMsr;
    WORD kmCsSelector;
    WORD umCsSelector;

    memzero(&starMsr, sizeof(IA32_STAR_MSR_DATA));

    kmCsSelector = GdtMuGetCS64Supervisor();
    ASSERT(kmCsSelector + 0x8 == GdtMuGetDS64Supervisor());

    umCsSelector = GdtMuGetCS32Usermode();
    /// DS64 is the same as DS32
    ASSERT(umCsSelector + 0x8 == GdtMuGetDS32Usermode());
    ASSERT(umCsSelector + 0x10 == GdtMuGetCS64Usermode());

    // Syscall RIP <- IA32_LSTAR
    AsmWriteMsr(IA32_LSTAR, (QWORD) SyscallEntry);

    LOG_TRACE_USERMODE("Successfully set LSTAR to 0x%X\n", (QWORD) SyscallEntry);

    // Syscall RFLAGS <- RFLAGS & ~(IA32_FMASK)
    AsmWriteMsr(IA32_FMASK, RFLAGS_INTERRUPT_FLAG_BIT);

    LOG_TRACE_USERMODE("Successfully set FMASK to 0x%X\n", RFLAGS_INTERRUPT_FLAG_BIT);

    // Syscall CS.Sel <- IA32_STAR[47:32] & 0xFFFC
    // Syscall DS.Sel <- (IA32_STAR[47:32] + 0x8) & 0xFFFC
    starMsr.SyscallCsDs = kmCsSelector;

    // Sysret CS.Sel <- (IA32_STAR[63:48] + 0x10) & 0xFFFC
    // Sysret DS.Sel <- (IA32_STAR[63:48] + 0x8) & 0xFFFC
    starMsr.SysretCsDs = umCsSelector;

    AsmWriteMsr(IA32_STAR, starMsr.Raw);

    LOG_TRACE_USERMODE("Successfully set STAR to 0x%X\n", starMsr.Raw);
}

// SyscallIdIdentifyVersion
STATUS
SyscallValidateInterface(
    IN  SYSCALL_IF_VERSION          InterfaceVersion
)
{
    LOG_TRACE_USERMODE("Will check interface version 0x%x from UM against 0x%x from KM\n",
        InterfaceVersion, SYSCALL_IF_VERSION_KM);

    if (InterfaceVersion != SYSCALL_IF_VERSION_KM)
    {
        LOG_ERROR("Usermode interface 0x%x incompatible with KM!\n", InterfaceVersion);
        return STATUS_INCOMPATIBLE_INTERFACE;
    }

    return STATUS_SUCCESS;
}

STATUS
SyscallProcessExit(
    IN      STATUS                  ExitStatus
)
{
    PPROCESS Process;
    Process = GetCurrentProcess();
    Process->TerminationStatus = ExitStatus;
    ProcessTerminate(Process);
    return STATUS_SUCCESS;
}

STATUS
SyscallThreadExit(
    IN  STATUS                      ExitStatus
)
{
    ThreadExit(ExitStatus);
    return STATUS_SUCCESS;
}
/*
STATUS
SyscallFileWrite(
    IN  UM_HANDLE                   FileHandle,
    IN_READS_BYTES(BytesToWrite)
    PVOID                       Buffer,
    IN  QWORD                       BytesToWrite,
    OUT QWORD* BytesWritten
)
{
    if (BytesWritten == NULL) {
        return STATUS_UNSUCCESSFUL;
    }

    if (FileHandle == UM_FILE_HANDLE_STDOUT) {
        *BytesWritten = BytesToWrite;
        LOG("[%s]:[%s]\n", ProcessGetName(NULL), Buffer);
        return STATUS_SUCCESS;
    }

    *BytesWritten = BytesToWrite;
    return STATUS_SUCCESS;
}
*/

STATUS
SyscallFileWrite(
    IN  UM_HANDLE                   FileHandle,
    IN_READS_BYTES(BytesToWrite)
    PVOID                           Buffer,
    IN  QWORD                       BytesToWrite,
    OUT QWORD* BytesWritten
)
{
    STATUS status;
    PPROCESS pProcess = GetCurrentProcess();

    // 1. Validăm pointerul pentru BytesWritten (fiind un parametru OUT, kernelul va scrie aici)
    // Este obligatoriu să verificăm dacă procesul are drept de scriere la acea adresă.
    status = MmuIsBufferValid(BytesWritten, sizeof(QWORD), PAGE_RIGHTS_WRITE, pProcess);
    if (!SUCCEEDED(status)) {
        return status;
    }

    // Inițializăm BytesWritten cu 0 în caz de eroare ulterioară
    *BytesWritten = 0;

    // 2. Validăm buffer-ul de date (fiind un parametru IN, kernelul doar citește din el)
    if (BytesToWrite > 0) {
        status = MmuIsBufferValid(Buffer, BytesToWrite, PAGE_RIGHTS_READ, pProcess);
        if (!SUCCEEDED(status)) {
            return status;
        }
    } else {
        return STATUS_SUCCESS;
    }

    // 3. Gestionarea cazului particular STDOUT (Cerința din PDF - 10%)
    if (FileHandle == UM_FILE_HANDLE_STDOUT) {
        // Afișăm conținutul buffer-ului folosind mecanismul de logare al kernelului.
        // Notă: Folosim %s presupunând că buffer-ul este un string, 
        // dar pentru siguranță maximă se poate folosi o funcție care scrie exact N octeți.
        //LOG("%s", (char*)Buffer);

        LOG("[%s] %s", GetCurrentProcess()->ProcessName, (char*)Buffer);

        // Confirmăm că am "scris" toți octeții pe ecran
        *BytesWritten = BytesToWrite;

        return STATUS_SUCCESS;
    }

    // 4. Dacă handle-ul nu este STDOUT, aici ar trebui să urmeze logica de scriere pe disc.
    // Dacă restul sistemului de fișiere nu este implementat, returnăm o eroare de handle invalid.
    return STATUS_INVALID_PARAMETER1;
}

// STUDENT TODO: implement the rest of the syscalls

// Obținerea Numelui Procesului
// Aceasta funcție verifică validitatea adresei și gestionează trunchierea numelui
STATUS
SyscallProcessGetName(
    OUT char* ProcessName,
    IN QWORD ProcessNameMaxLen
)
{
    STATUS status;
    PPROCESS pProcess = GetCurrentProcess();
    QWORD nameLen;

    // Verificăm dacă buffer-ul este valid în spațiul utilizator
    status = MmuIsBufferValid(ProcessName, ProcessNameMaxLen, PAGE_RIGHTS_WRITE, pProcess);
    if (!SUCCEEDED(status)) {
        return status;
    }

    if (ProcessNameMaxLen == 0) return STATUS_INVALID_PARAMETER2;

    // Copiem numele procesului (presupunem că pProcess->Name este stocat în kernel)
    const char* actualName = ProcessGetName(pProcess);
    nameLen = strlen(actualName);

    if (nameLen >= ProcessNameMaxLen) {
        // Trunchiem numele și adăugăm null-terminator
        memcpy(ProcessName, actualName, ProcessNameMaxLen - 1);
        ProcessName[ProcessNameMaxLen - 1] = '\0';
        return STATUS_TRUNCATED_PROCESS_NAME;
    }

    strcpy(ProcessName, actualName);
    return STATUS_SUCCESS;
}

// Gestiunea Priorității și CPU-ului
// Implementări simple pentru returnarea datelor despre firul curent și hardware
STATUS
SyscallGetThreadPriority(
    OUT BYTE* ThreadPriority
)
{
    /*
    if (ThreadPriority == NULL) return STATUS_INVALID_PARAMETER1;
    *ThreadPriority = GetCurrentThread()->Priority;
    return STATUS_SUCCESS;
    */
    STATUS status;
    PPROCESS pProcess = GetCurrentProcess();

    status = MmuIsBufferValid(ThreadPriority, sizeof(BYTE), PAGE_RIGHTS_WRITE, pProcess);
    if (!SUCCEEDED(status)) return status;

    *ThreadPriority = GetCurrentThread()->Priority;
    return STATUS_SUCCESS;
}

STATUS
SyscallSetThreadPriority(
    IN BYTE ThreadPriority
)
{
    // Setăm prioritatea firului curent 
    if (ThreadPriority > 31) return STATUS_INVALID_PARAMETER1;

    GetCurrentThread()->Priority = ThreadPriority;
    return STATUS_SUCCESS;
}

STATUS
SyscallGetCurrentCPUID(
    OUT BYTE* CpuId
)
{
    STATUS status;
    PPROCESS pProcess = GetCurrentProcess();

    status = MmuIsBufferValid(CpuId, sizeof(BYTE), PAGE_RIGHTS_WRITE, pProcess);
    if (!SUCCEEDED(status)) return status;

    *CpuId = (BYTE)GetCurrentPcpu()->ApicId;
    return STATUS_SUCCESS;
}

// Utilizarea CPU
// Calculul se bazează pe formula: ((total - idle) / total) * 100
/*STATUS
SyscallGetCPUUtilization(
    IN_OPT BYTE* CpuId,
    OUT BYTE* Utilization
)
{
    if (Utilization == NULL) return STATUS_INVALID_PARAMETER2;

    // Formula: ((total ticks - idle ticks) / total ticks) * 100
    // Total Ticks = KernelTicks + IdleTicks
    
    if (CpuId == NULL) {
        // Media tuturor procesoarelor
        // Notă: Va trebui să iterați prin lista m_threadSystemData.AllThreadsList 
        // sau să folosiți gEntData.CpuCount dacă este accesibil.
        *Utilization = 0; 
    } else {
        // Pentru un CPU specific. Notă: În HAL9000, GetCurrentPcpu() returnează CPU-ul curent.
        // Pentru alte CPU-uri, ar trebui parcursă lista pcpu din sistem.
        PPCPU pPcpu = GetCurrentPcpu();
        QWORD idle = pPcpu->ThreadData.IdleTicks;
        QWORD kernel = pPcpu->ThreadData.KernelTicks;
        QWORD total = idle + kernel;

        if (total > 0) {
            *Utilization = (BYTE)((kernel * 100) / total);
        } else {
            *Utilization = 0;
        }
    }
    return STATUS_SUCCESS;
}*/

STATUS
SyscallGetCPUUtilization(
    IN_OPT BYTE* CpuId,
    OUT BYTE* Utilization
)
{
    STATUS status;
    PPROCESS pProcess = GetCurrentProcess();
    PLIST_ENTRY pCpuListHead;
    PLIST_ENTRY pEntry;
    QWORD idleTicks;
    QWORD kernelTicks;
    QWORD totalTicks;
    BOOLEAN cpuFound;

    status = MmuIsBufferValid(Utilization, sizeof(BYTE), PAGE_RIGHTS_WRITE, pProcess);
    if (!SUCCEEDED(status))
    {
        return status;
    }

    pCpuListHead = NULL;
    pEntry = NULL;
    idleTicks = 0;
    kernelTicks = 0;
    cpuFound = FALSE;

    if (CpuId != NULL)
    {
        BYTE requestedCpuId;

        status = MmuIsBufferValid((PVOID)CpuId, sizeof(BYTE), PAGE_RIGHTS_READ, pProcess);
        if (!SUCCEEDED(status))
        {
            return status;
        }

        requestedCpuId = *CpuId;

        SmpGetCpuList(&pCpuListHead);
        for (pEntry = pCpuListHead->Flink;
             pEntry != pCpuListHead;
             pEntry = pEntry->Flink)
        {
            PPCPU pCpu = CONTAINING_RECORD(pEntry, PCPU, ListEntry);
            if ((BYTE)pCpu->ApicId == requestedCpuId)
            {
                idleTicks = pCpu->ThreadData.IdleTicks;
                kernelTicks = pCpu->ThreadData.KernelTicks;
                cpuFound = TRUE;
                break;
            }
        }

        if (!cpuFound)
        {
            return STATUS_INVALID_PARAMETER1;
        }
    }
    else
    {
        SmpGetCpuList(&pCpuListHead);
        for (pEntry = pCpuListHead->Flink;
             pEntry != pCpuListHead;
             pEntry = pEntry->Flink)
        {
            PPCPU pCpu = CONTAINING_RECORD(pEntry, PCPU, ListEntry);
            idleTicks += pCpu->ThreadData.IdleTicks;
            kernelTicks += pCpu->ThreadData.KernelTicks;
        }
    }

    totalTicks = idleTicks + kernelTicks;
    if (totalTicks > 0)
    {
        *Utilization = (BYTE)((kernelTicks * 100) / totalTicks);
    }
    else
    {
        *Utilization = 0;
    }

    return STATUS_SUCCESS;
}

STATUS 
SyscallThreadGetTid(
    IN_OPT  UM_HANDLE   ThreadHandle,
    OUT     TID* ThreadId
)
{
    STATUS status;
    PPROCESS pProcess = GetCurrentProcess();
    PTHREAD pThread;

    status = MmuIsBufferValid(ThreadId, sizeof(TID), PAGE_RIGHTS_WRITE, pProcess);
    if (!SUCCEEDED(status))
    {
        return status;
    }

    if (ThreadHandle == UM_INVALID_HANDLE_VALUE)
    {
        pThread = NULL;
    }
    else
    {
        pThread = (PTHREAD)ThreadHandle;
    }

    *ThreadId = ThreadGetId(pThread);
    return STATUS_SUCCESS;
}

STATUS
SyscallVirtualAlloc(
    IN_OPT      PVOID                   BaseAddress,
    IN          QWORD                   Size,
    IN          VMM_ALLOC_TYPE          AllocType,
    IN          PAGE_RIGHTS             PageRights,
    IN_OPT      UM_HANDLE               FileHandle,
    IN_OPT      QWORD                   Key,
    OUT         PVOID*                  AllocatedAddress
)
{
    STATUS status;
    PPROCESS pProcess = GetCurrentProcess();

    // Validate output buffer
    status = MmuIsBufferValid(AllocatedAddress, sizeof(PVOID), PAGE_RIGHTS_WRITE, pProcess);
    if (!SUCCEEDED(status))
    {
        return status;
    }

    // Check required conditions: BaseAddress == NULL, FileHandle == UM_INVALID_HANDLE_VALUE, Key == 0
    if (BaseAddress != NULL)
    {
        return STATUS_INVALID_PARAMETER1;
    }
    if (FileHandle != UM_INVALID_HANDLE_VALUE)
    {
        return STATUS_INVALID_PARAMETER5;
    }
    if (Key != 0)
    {
        return STATUS_INVALID_PARAMETER6;
    }

    // Use VmmAllocRegionEx to allocate memory
    *AllocatedAddress = VmmAllocRegionEx(
        NULL,                   // Let kernel decide address
        Size,
        AllocType,
        PageRights,
        FALSE,                  // Not uncachable
        NULL,                   // No file backing
        pProcess->VaSpace,      // Process VA space
        NULL,                   // No MDL
        NULL                    // No name
    );

    if (*AllocatedAddress == NULL)
    {
        return STATUS_MEMORY_CANNOT_BE_COMMITED;
    }

    return STATUS_SUCCESS;
}

STATUS
SyscallVirtualFree(
    IN          PVOID                   Address,
    _When_(VMM_FREE_TYPE_RELEASE == FreeType, _Reserved_)
    _When_(VMM_FREE_TYPE_RELEASE != FreeType, IN)
                QWORD                   Size,
    IN          VMM_FREE_TYPE           FreeType
)
{
    if (Address == NULL)
    {
        return STATUS_INVALID_PARAMETER1;
    }

    VmmFreeRegionEx(Address, Size, FreeType, TRUE, NULL, NULL);
    return STATUS_SUCCESS;
}

STATUS
SyscallGetPageFaultNo(
    IN      PVOID                   AllocatedVirtAddr,
    OUT     QWORD*                  PageFaultNo
)
{
    STATUS status;
    PPROCESS pProcess = GetCurrentProcess();
    PHYSICAL_ADDRESS physAddr;

    // Validate output buffer
    status = MmuIsBufferValid(PageFaultNo, sizeof(QWORD), PAGE_RIGHTS_WRITE, pProcess);
    if (!SUCCEEDED(status))
    {
        return status;
    }

    // Validate input address
    if (AllocatedVirtAddr == NULL)
    {
        return STATUS_INVALID_PARAMETER1;
    }

    // Get physical address to check if page is mapped
    physAddr = MmuGetPhysicalAddressEx(
        AllocatedVirtAddr,
        pProcess->PagingData,
        NULL
    );

    if (physAddr == 0)
    {
        // Page not mapped
        *PageFaultNo = 0;
        return STATUS_SUCCESS;
    }

    // Get page fault count from current PCPU
    *PageFaultNo = GetCurrentPcpu()->PageFaults;
    return STATUS_SUCCESS;
}

STATUS
SyscallGetPagePhysAddr(
    IN      PVOID                   AllocatedVirtAddr,
    OUT     PVOID*                  AllocatedPhysAddr
)
{
    STATUS status;
    PPROCESS pProcess = GetCurrentProcess();
    PHYSICAL_ADDRESS physAddr;

    // Validate output buffer
    status = MmuIsBufferValid(AllocatedPhysAddr, sizeof(PVOID), PAGE_RIGHTS_WRITE, pProcess);
    if (!SUCCEEDED(status))
    {
        return status;
    }

    // Validate input address
    if (AllocatedVirtAddr == NULL)
    {
        return STATUS_INVALID_PARAMETER1;
    }

    // Get physical address mapping
    physAddr = MmuGetPhysicalAddressEx(
        AllocatedVirtAddr,
        pProcess->PagingData,
        NULL
    );

    if (physAddr == 0)
    {
        // Page not mapped
        *AllocatedPhysAddr = NULL;
        return STATUS_SUCCESS;
    }

    *AllocatedPhysAddr = (PVOID)physAddr;
    return STATUS_SUCCESS;
}

STATUS
SyscallGetPageInternalFragmentation(
    IN      PVOID                   AllocatedVirtAddr,
    OUT     QWORD*                  IntFragSize
)
{
    STATUS status;
    PPROCESS pProcess = GetCurrentProcess();
    PHYSICAL_ADDRESS physAddr;
    PVOID pageBase;
    QWORD offsetInPage;

    // Validate output buffer
    status = MmuIsBufferValid(IntFragSize, sizeof(QWORD), PAGE_RIGHTS_WRITE, pProcess);
    if (!SUCCEEDED(status))
    {
        return status;
    }

    // Validate input address
    if (AllocatedVirtAddr == NULL)
    {
        return STATUS_INVALID_PARAMETER1;
    }

    // Get physical address to check if page is mapped
    physAddr = MmuGetPhysicalAddressEx(
        AllocatedVirtAddr,
        pProcess->PagingData,
        NULL
    );

    if (physAddr == 0)
    {
        // Page not mapped
        *IntFragSize = 0;
        return STATUS_SUCCESS;
    }

    // Calculate internal fragmentation
    // Page base address (aligned to PAGE_SIZE)
    pageBase = (PVOID)((QWORD)AllocatedVirtAddr & ~(PAGE_SIZE - 1));
    offsetInPage = (QWORD)AllocatedVirtAddr - (QWORD)pageBase;

    // Internal fragmentation is unused space at end of the page
    *IntFragSize = PAGE_SIZE - offsetInPage;
    return STATUS_SUCCESS;
}

STATUS
SyscallGetNumberOfThreadsForCurrentProcess(
    OUT QWORD* ThreadNo
)
{
    STATUS status;
    PPROCESS pProcess = GetCurrentProcess();

    // 1. Validăm pointerul de ieșire (ThreadNo)
    // Este critic să verificăm dacă adresa aparține spațiului de memorie al utilizatorului
    // și dacă avem drepturi de scriere (PAGE_RIGHTS_WRITE).
    status = MmuIsBufferValid(ThreadNo, sizeof(QWORD), PAGE_RIGHTS_WRITE, pProcess);
    if (!SUCCEEDED(status)) {
        // Dacă adresa este invalidă (ex: 0x1234), kernelul returnează eroarea fără să crape
        return status;
    }

    // 2. Extragem numărul de thread-uri din structura procesului
    // Folosim NumberOfThreads deoarece acesta reprezintă totalul firelor pornite
    // care sunt încă înregistrate în proces (chiar dacă unele s-au terminat dar nu au fost 'join-uite').
    *ThreadNo = (QWORD)pProcess->NumberOfThreads;

    return STATUS_SUCCESS;
}