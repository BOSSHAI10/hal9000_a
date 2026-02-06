#include "common_lib.h"
#include "syscall_if.h"
#include "um_lib_helper.h"
#include "syscall_no.h"
#include "syscall_func.h"
#include "status.h"
#include "cl_string.h"
#include "assert.h"

#define printf(...) LOG(__VA_ARGS__)

// Poarta de intrare către Kernel
extern STATUS SyscallEntry(SYSCALL_ID SyscallId, ...);

// --- Implementări Syscall (Capitolul 2) ---
STATUS SyscallProcessGetName(OUT char* Name, IN QWORD Len) { return SyscallEntry(SyscallIdProcessGetName, Name, Len); }
STATUS SyscallGetNumberOfThreadsForCurrentProcess(OUT QWORD* No) { return SyscallEntry(SyscallIdGetNumberOfThreadsForCurrentProcess, No); }
STATUS SyscallGetCPUUtilization(IN_OPT BYTE* Id, OUT BYTE* Utilization) { return SyscallEntry(SyscallIdGetCPUUtilization, Id, Utilization); }

// --- Test Memorie Virtuală ---
STATUS SyscallVirtualAlloc(IN_OPT PVOID BaseAddress, IN QWORD Size, IN DWORD AllocType, IN DWORD PageRights, IN_OPT UM_HANDLE FileHandle, IN_OPT QWORD Key, OUT PVOID* AllocatedAddress);
STATUS SyscallVirtualFree(IN PVOID Address, IN QWORD Size, IN DWORD FreeType);
STATUS SyscallGetPageFaultNo(IN PVOID AllocatedVirtAddr, OUT QWORD* PageFaultNo);
STATUS SyscallGetPagePhysAddr(IN PVOID AllocatedVirtAddr, OUT PVOID* AllocatedPhysAddr);
STATUS SyscallGetPageInternalFragmentation(IN PVOID AllocatedVirtAddr, OUT QWORD* IntFragSize);

#define PGSIZE 4096
#define SIZE (3 * PGSIZE)
#define PtrOff(ptr,off) (((PBYTE)(ptr)) + ((QWORD)(off)))

STATUS __main(DWORD Argc, char** Argv)
{
    UNREFERENCED_PARAMETER(Argc);
    UNREFERENCED_PARAMETER(Argv);
    PVOID allocatedVirtAddr;
    PBYTE pg, off;
    PVOID allocatedPhysAddr;
    QWORD i, pageFaultNo, pageIntFrag;
    STATUS status;

    printf("\n--- LightProjectApp: TESTARE MEMORIE VIRTUALA ---\n");

    for (i = 0; i <= PGSIZE; i += PGSIZE / 4)
    {
        // allocated some memory (not always a multiple of page size)
        if (((i / (PGSIZE / 4)) % 2 == 0))
        {
            LOG("Allocate %d bytes, covered by %d pages\n", SIZE + i, (SIZE + i) / PGSIZE + ((SIZE + i) % PGSIZE == 0 ? 0 : 1));
            status = SyscallVirtualAlloc(NULL, SIZE + i, VMM_ALLOC_TYPE_RESERVE | VMM_ALLOC_TYPE_COMMIT, PAGE_RIGHTS_READ | PAGE_RIGHTS_WRITE, UM_INVALID_HANDLE_VALUE, 0, &allocatedVirtAddr);
        }
        else
        {
            status = SyscallVirtualAlloc(NULL, SIZE + i, VMM_ALLOC_TYPE_RESERVE | VMM_ALLOC_TYPE_COMMIT | VMM_ALLOC_TYPE_NOT_LAZY, PAGE_RIGHTS_READ | PAGE_RIGHTS_WRITE, UM_INVALID_HANDLE_VALUE, 0, &allocatedVirtAddr);
        }

        if (!SUCCEEDED(status))
        {
            LOG("Cannot allocate memory: err status = %d\n", status);
            return status;
        }

        // get access (write) to allocated memory, byte by byte
        for (off = allocatedVirtAddr; off < PtrOff(allocatedVirtAddr, SIZE + i); off += 1)
            *off = 10;

        // get info about allocated memory, page by page
        for (pg = allocatedVirtAddr; pg < PtrOff(allocatedVirtAddr, SIZE + i); pg += PGSIZE)
        {
            SyscallGetPagePhysAddr(pg, &allocatedPhysAddr);
            LOG("AllocatedPhysAddr = %X for AllocatedVirtAddr = %X", allocatedPhysAddr, pg);
            
            SyscallGetPageFaultNo(pg, &pageFaultNo);
            LOG("PageFaultNo = %u for VirtAddr = %X", pageFaultNo, pg);
            
            SyscallGetPageInternalFragmentation(pg, &pageIntFrag);
            LOG("InternalFrag = %u for VirtAddr = %X", pageIntFrag, pg);
        }

        // release allocated memory
        SyscallVirtualFree(allocatedVirtAddr, 0, VMM_FREE_TYPE_RELEASE);
        status = SyscallGetPagePhysAddr(allocatedVirtAddr, &allocatedPhysAddr);
        if (!SUCCEEDED(status))
        {
            LOG("AllocatedVirtAddr = %X is not mapped anymore", allocatedVirtAddr);
        }
        else
        {
            LOG("Error: AllocatedVirtAddr = %X still mapped on AllocatedPhysAddr = %X after being released", allocatedVirtAddr, allocatedVirtAddr);
        }
    }

    return STATUS_SUCCESS;
}
