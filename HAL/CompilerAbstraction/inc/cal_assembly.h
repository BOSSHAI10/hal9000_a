#pragma once

#include "cal_compiler.h"
#include "cal_types.h"

// I really could not find a better solution which
// supports both MSVC and gcc/clang

void
AsmCpuid(
    int CpuInfo[4],
    int FunctionId
    );

void
AsmCpuidex(
    int CpuInfo[4],
    int FunctionId,
    int SubFunctionId
    );

void
AsmHalt(
    void
    ); 

QWORD
AsmReadEflags(
    void
    );

void
AsmWriteEflags(
    QWORD Rflags
    );

BYTE
AsmInByte(
    WORD Port
    );

void 
AsmOutByte(
    WORD Port,
    BYTE Data
    );

WORD
AsmInWord(
    WORD Port
    );

void 
AsmOutWord(
    WORD Port,
    WORD Data
    );

DWORD
AsmInDword(
    WORD Port
    );

void
AsmOutDword(
    WORD Port,
    DWORD Data
    );

QWORD 
AsmReadMsr(
    DWORD Register
    );

void
AsmWriteMsr(
    DWORD Register,
    QWORD Value
    );

void
AsmMovsq(
    void *Destination,
    void * const Source,
    QWORD const Count
    );

QWORD 
AsmReadCr0(
    void
    );

void
AsmWriteCr0(
    QWORD Cr0
    );

// QWORD 
// AsmReadCr1(
//     void
//     );

// void 
// AsmWriteCr1(
//     QWORD Cr1
//     );

QWORD 
AsmReadCr2(
    void
    );

void
AsmWriteCr2(
    QWORD Cr2
    );

QWORD
AsmReadCr3(
    void
    ); 

void
AsmWriteCr3(
    QWORD Cr3
    );

QWORD
AsmReadCr4(
    void
    );

void
AsmWriteCr4(
    QWORD Cr4
    );

QWORD
AsmReadCr8(
    void
    );

void
AsmWriteCr8(
    QWORD Cr8
    );

void
AsmInvlpg(
    void *Address
    );

void
AsmLidt(
    void *Source
    );

QWORD
AsmRdtsc(
    void
    );

void
AsmXsetbv(
    DWORD Register,
    QWORD Value
    );

QWORD
AsmReadFsQword(
    DWORD Offset
    );

QWORD
AsmReadGsQword(
    DWORD Offset
    );

void
AsmPause(
    void
    );

// Needs -msse3
void
AsmMwait(
    unsigned int Extensions,
    unsigned int Hints
    );

// Needs -msse3
void
AsmMonitor(
    void const* P,
    DWORD Extensions,
    DWORD Hints
    );

// Needs -msse2
void AsmLfence(
    void
    );

BYTE AsmVmxVmread(
    QWORD Field,
    QWORD *FieldValue
    );

BYTE AsmVmxVmwrite(
    QWORD Field,
    QWORD FieldValue
    );
