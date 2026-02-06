#include "cal_assembly.h"

#include "cal_annotate.h"

#ifdef CAL_MSVC

void
AsmCpuid(
    int CpuInfo[4],
    int FunctionId
    )
{
    __cpuid(CpuInfo, FunctionId);
}

void
AsmCpuidex(
    int CpuInfo[4],
    int FunctionId,
    int SubFunctionId
    )
{
    __cpuidex(CpuInfo, FunctionId, SubFunctionId);
}

void
AsmHalt(
    void
    )
{
    __halt();
}

QWORD
AsmReadEflags(
    void
    )
{
    return __readeflags();
}

void
AsmWriteEflags(
    QWORD Rflags
    )
{
    __writeeflags(Rflags);
}

BYTE
AsmInByte(
    WORD Port
    )
{
    return __inbyte(Port);
}

void 
AsmOutByte(
    WORD Port,
    BYTE Data
    )
{
    __outbyte(Port, Data);
}

WORD
AsmInWord(
    WORD Port
    )
{
    return __inword(Port);
}

void 
AsmOutWord(
    WORD Port,
    WORD Data
    )
{
    __outword(Port, Data);
}

DWORD
AsmInDword(
    WORD Port
    )
{
    return __indword(Port);
}

void
AsmOutDword(
    WORD Port,
    DWORD Data
    )
{
    __outdword(Port, Data);
}

QWORD 
AsmReadMsr(
    DWORD Register
    )
{
    return __readmsr(Register);
}

void
AsmWriteMsr(
    DWORD Register,
    QWORD Value
    )
{
    __writemsr(Register, Value);
}

void
AsmMovsq(
    void *Destination,
    void * const Source,
    QWORD const Count
    )
{
    __movsq(Destination, Source, Count);
}

QWORD 
AsmReadCr0(
    void
    )
{
    return __readcr0();
}

void
AsmWriteCr0(
    QWORD Cr0
    )
{
    __writecr0(Cr0);
}

// QWORD 
// AsmReadCr1(
//     void
//     )
// {
//     return __readcr1();
// }

// void 
// AsmWriteCr1(
//     QWORD Cr1
//     )
// {
//     __writecr1(Cr1);
// }

QWORD 
AsmReadCr2(
    void
    )
{
    return __readcr2();
}


void
AsmWriteCr2(
    QWORD Cr2
    )
{
    __writecr2(Cr2);
}

QWORD
AsmReadCr3(
    void
    )
{
    return __readcr3();
}

void
AsmWriteCr3(
    QWORD Cr3
    )
{
    __writecr3(Cr3);
}

QWORD
AsmReadCr4(
    void
    )
{
    return __readcr4();
}

void
AsmWriteCr4(
    QWORD Cr4
    )
{
    __writecr4(Cr4);
}

QWORD
AsmReadCr8(
    void
    )
{
    return __readcr8();
}

void
AsmWriteCr8(
    QWORD Cr8
    )
{
    __writecr8(Cr8);
}

void
AsmInvlpg(
    void *Address
    )
{
    __invlpg(Address);
}

void
AsmLidt(
    void *Source
    )
{
    __lidt(Source);
}

QWORD
AsmRdtsc(
    void
    )
{
    return __rdtsc();
}

void
AsmXsetbv(
    DWORD Register,
    QWORD Value
    )
{
    _xsetbv(Register, Value);
}

typedef
QWORD
(CDECL MS_ABI FUNC_ReadFsQword)(
    DWORD Offset
    );

extern FUNC_ReadFsQword __CALreadfsqword;

QWORD
AsmReadFsQword(
    DWORD Offset
    )
{
    return __CALreadfsqword(Offset);
}

QWORD
AsmReadGsQword(
    DWORD Offset
    )
{
    return __readgsqword(Offset);
}

void
AsmPause(
    void
    )
{
    _mm_pause();
}

void
AsmMwait(
    unsigned int Extensions,
    unsigned int Hints
    )
{
    _mm_mwait(Extensions, Hints);
}

void
AsmMonitor(
    void const* P,
    DWORD Extensions,
    DWORD Hints
    )
{
    _mm_monitor(P, Extensions, Hints);
}

void AsmLfence(
    void
    )
{
    _mm_lfence();
}

BYTE AsmVmxVmread(
    QWORD Field,
    QWORD *FieldValue
    )
{
    return __vmx_vmread(Field, FieldValue);
}

BYTE AsmVmxVmwrite(
    QWORD Field,
    QWORD FieldValue
    )
{
    return __vmx_vmwrite(Field, FieldValue);
}

#else

void
AsmCpuid(
    int CpuInfo[4],
    int FunctionId
    )
{
    QWORD Rax, Rbx, Rcx, Rdx;
    __asm__ __volatile__ (
        "cpuid"
        : "=a" (Rax), "=b" (Rbx), "=c" (Rcx), "=d" (Rdx)
        : "a" ((QWORD) FunctionId)
        : "memory"
    );
    CpuInfo[0] = (DWORD) (Rax & UINT32_MAX);
    CpuInfo[1] = (DWORD) (Rbx & UINT32_MAX);
    CpuInfo[2] = (DWORD) (Rcx & UINT32_MAX);
    CpuInfo[3] = (DWORD) (Rdx & UINT32_MAX);
}

void
AsmCpuidex(
    int CpuInfo[4],
    int FunctionId,
    int SubFunctionId
    )
{
    QWORD Rax, Rbx, Rcx, Rdx;
    __asm__ __volatile__ (
        "cpuid"
        : "=a" (Rax), "=b" (Rbx), "=c" (Rcx), "=d" (Rdx)
        : "a" ((QWORD) FunctionId), "c" ((QWORD) SubFunctionId)
        : "memory"
    );
    CpuInfo[0] = (DWORD) (Rax & UINT32_MAX);
    CpuInfo[1] = (DWORD) (Rbx & UINT32_MAX);
    CpuInfo[2] = (DWORD) (Rcx & UINT32_MAX);
    CpuInfo[3] = (DWORD) (Rdx & UINT32_MAX);
}

void
AsmHalt(
    void
    ) 
{
    __asm__ __volatile__ (
        "hlt"
        :
        :
        : "memory"
    );
}

QWORD
AsmReadEflags(
    void
    )
{
    QWORD ret;
    __asm__ __volatile__ (
        "pushfq; pop %0"
        : "=r" (ret)
        :
        : "memory"
    );
    return ret;
}

void
AsmWriteEflags(
    QWORD Rflags
    )
{
    __asm__ __volatile__ (
        "push %0; popfq"
        :
        : "r" (Rflags)
        : "memory"
    );
}

BYTE
AsmInByte(
    WORD Port
    )
{
    BYTE Data;
    __asm__ __volatile__ (
        "inb %1, %b0"
        : "=a"(Data)
        : "d"(Port)
        : "memory"
    );
    return Data;
}

void 
AsmOutByte(
    WORD Port,
    BYTE Data
    )
{
    __asm__ __volatile__ (
        "outb %b0, %1"
        :
        : "a"(Data), "d"(Port)
        : "memory"
    );
}

WORD
AsmInWord(
    WORD Port
    )
{
    WORD Data;
    __asm__ __volatile__ (
        "in %1, %w0"
        : "=a"(Data)
        : "d"(Port)
        : "memory"
    );
    return Data;
}

void 
AsmOutWord(
    WORD Port,
    WORD Data
    )
{
    __asm__ __volatile__ (
        "out %w0, %1"
        :
        : "a"(Data), "d"(Port)
        : "memory"
    );
}

DWORD
AsmInDword(
    WORD Port
    )
{
    DWORD Data;
    __asm__ __volatile__ (
        "in %1, %0"
        : "=a"(Data)
        : "d"(Port)
        : "memory"
    );
    return Data;
}

void 
AsmOutDword(
    WORD Port,
    DWORD Data
    )
{
    __asm__ __volatile__ (
        "out %0, %1"
        :
        : "a"(Data), "d"(Port)
        : "memory"
    );
}

QWORD 
AsmReadMsr(
    DWORD Register
    )
{
    QWORD Rdx, Rax;
    __asm__ __volatile__ (
        "xor %%rax, %%rax; xor %%rdx, %%rdx; rdmsr"
        : "=d" (Rdx), "=a" (Rax)
        : "c" (Register)
        : "memory"
    );
    return ((Rdx & UINT32_MAX) << 32) | (Rax & UINT32_MAX);
}

void
AsmWriteMsr(
    DWORD Register,
    QWORD Value
    )
{
    QWORD Rdx = Value >> 32;
    QWORD Rax = Value & UINT32_MAX;
    __asm__ __volatile__(
        "wrmsr"
        :
        : "c" (Register), "d" (Rdx), "a" (Rax)
        : "memory"
    );
}

void
AsmMovsq(
    void *Destination,
    void * const Source,
    QWORD const Count
    )
{
    __asm__ __volatile__(
        "cld; rep movsq"
        :
        : "S" (Source), "D" (Destination), "c" (Count)
        : "cc", "memory"
    );
}

QWORD 
AsmReadCr0(
    void
    )
{
    QWORD Ret;
    __asm__ __volatile__ (
        "mov %%cr0, %0"
        : "=r" (Ret)
        :
        : "memory"
    );
    return Ret;
}

inline
void
AsmWriteCr0(
    QWORD Cr0
    )
{
    __asm__ __volatile__ (
        "mov %0, %%cr0"
        :
        : "r" (Cr0)
        : "memory"
    );
}

// QWORD 
// AsmReadCr1(
//     void
//     )
// {
//     QWORD Ret;
//     __asm__ __volatile__ (
//         "mov %%cr1, %0"
//         : "=r" (Ret)
//         :
//         : "memory"
//     );
//     return Ret;
// }

// void 
// AsmWriteCr1(
//     QWORD Cr1
//     )
// {
//     __asm__ __volatile__ (
//         "mov %0, %%cr1"
//         :
//         : "r" (Cr1)
//         : "memory"
//     );
// }

QWORD 
AsmReadCr2(
    void
    )
{
    QWORD Ret;
    __asm__ __volatile__ (
        "mov %%cr2, %0"
        : "=r" (Ret)
        :
        : "memory"
    );
    return Ret;
}

void
AsmWriteCr2(
    QWORD Cr2
    )
{
    __asm__ __volatile__ (
        "mov %0, %%cr2"
        :
        : "r" (Cr2)
        : "memory"
    );
}

QWORD
AsmReadCr3(
    void
    ) 
{
    QWORD Ret;
    __asm__ __volatile__ (
        "mov %%cr3, %0"
        : "=r" (Ret)
        :
        : "memory"
    );
    return Ret;
}

void
AsmWriteCr3(
    QWORD Cr3
    )
{
    __asm__ __volatile__ (
        "mov %0, %%cr3"
        :
        : "r" (Cr3)
        : "memory"
    );
}

QWORD
AsmReadCr4(
    void
    )
{
    QWORD Ret;
    __asm__ __volatile__ (
        "mov %%cr4, %0"
        : "=r" (Ret)
        :
        : "memory"
    );
    return Ret;
}

void
AsmWriteCr4(
    QWORD Cr4
    )
{
    __asm__ __volatile__ (
        "mov %0, %%cr4"
        :
        : "r" (Cr4)
        : "memory"
    );
}

QWORD
AsmReadCr8(
    void
    )
{
    QWORD Ret;
    __asm__ __volatile__ (
        "mov %%cr8, %0"
        : "=r" (Ret)
        :
        : "memory"
    );
    return Ret;
}

void
AsmWriteCr8(
    QWORD Cr8
    )
{
    __asm__ __volatile__ (
        "mov %0, %%cr8"
        :
        : "r" (Cr8)
        : "memory"
    );
}

void
AsmInvlpg(
    void *Address
    )
{
    __asm__ __volatile__ (
        "invlpg (%0)"
        :
        : "r" ((QWORD) Address)
        : "memory"
    );
}

void
AsmLidt(
    void *Source
    )
{
    __asm__ __volatile__ (
        "lidt (%0)"
        :
        : "r" ((QWORD) Source)
        : "memory"
    );
}

QWORD
AsmRdtsc(
    void
    )
{
    QWORD Rdx, Rax;
    __asm__ __volatile__ (
        "rdtsc"
        : "=d" (Rdx), "=a" (Rax)
        :
        : "memory"
    ); 
    return ((Rdx & UINT32_MAX) << 32) | (Rax & UINT32_MAX);
}

void
AsmXsetbv(
    DWORD Register,
    QWORD Value
    )
{
    QWORD Rdx = Value >> 32;
    QWORD Rax = Value & UINT32_MAX;
    __asm__ __volatile__ (
        "xsetbv"
        :
        : "c" (Register), "d" (Rdx), "a" (Rax)
        : "memory"
    );
}

QWORD
AsmReadFsQword(
    DWORD Offset
    )
{
    return *((QWORD *)(((BYTE *) AsmReadMsr(0xC0000100)) + Offset));
}

QWORD
AsmReadGsQword(
    DWORD Offset
    )
{
    return *((QWORD *)(((BYTE *) AsmReadMsr(0xC0000101)) + Offset));
}

void
AsmPause(
    void
    )
{
    __builtin_ia32_pause();
}

void
AsmMwait(
    unsigned int Extensions,
    unsigned int Hints
    )
{
    __asm__ __volatile__ (
        "mwait"
        :
        : "c"(Extensions), "a"(Hints)
        : "memory"
    );
}

void
AsmMonitor(
    void const* P,
    DWORD Extensions,
    DWORD Hints
    )
{
    __asm__ __volatile__ (
        "monitor"
        :
        : "a"(P), "c"(Extensions), "d"(Hints)
        : "memory"
    );
}

void AsmLfence(
    void
    )
{
    __asm__ __volatile__ (
        "lfence"
        :
        :
        : "memory"
    );
}

BYTE AsmVmxVmread(
    QWORD Field,
    QWORD *FieldValue
    )
{
    QWORD flags;
    __asm__ __volatile__ ("vmread %1, (%2); pushfq; pop %0"
        : "=r" (flags) 
        : "r" (Field), "r" (FieldValue) 
        : "memory"
    );
    if (flags & 1)
        return 2;
    if (flags & (1 << 6))
        return 1;
    return 0;
}

BYTE AsmVmxVmwrite(
    QWORD Field,
    QWORD FieldValue
    )
{
    QWORD flags;
    __asm__ __volatile__ ("vmwrite %2, %1; pushfq; pop %0"
        : "=r" (flags) 
        : "r" (Field), "r" (FieldValue) 
        : "memory"
    );
    if (flags & 1)
        return 2;
    if (flags & (1 << 6))
        return 1;
    return 0;
}

#endif
