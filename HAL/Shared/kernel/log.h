#pragma once

#include "native/string.h"
#include "cpu.h"
#include "cal_vargs.h"

//#define NO_COMM

#ifndef NO_COMM

// #define LOG_ATC(lvl,comp,buf,...)               LogEx((lvl),(comp),buf,__VA_ARGS__)

// #define LOG_AT(lvl,buf,...)                     LOG_ATC((lvl),LogComponentGeneric,(buf),__VA_ARGS__)
// #define LOG_ATLC(lvl,comp,buf,...)              LOG_ATC((lvl),(comp),"[%s][%d]"##buf, strrchr(__FILE__,'\\') + 1, __LINE__, __VA_ARGS__)

// #define LOG(buf,...)                            LOG_AT(LogLevelInfo, buf, __VA_ARGS__)
// #define LOGP(buf,...)                           LOG_AT(LogLevelInfo, "[CPU:%02x]"##buf, CpuGetApicId(), __VA_ARGS__ )
// #define LOGP_WARNING(buf,...)                   LOG_ATLC(LogLevelWarning, LogComponentGeneric, "[CPU:%02x]"##buf, CpuGetApicId(), __VA_ARGS__ )
// #define LOGP_ERROR(buf,...)                     LOG_ATLC(LogLevelError, LogComponentGeneric, "[CPU:%02x]"##buf, CpuGetApicId(), __VA_ARGS__ )

// #define LOGL(buf,...)                           LOG_ATLC( LogLevelInfo, LogComponentGeneric, buf, __VA_ARGS__ )
// #define LOGPL(buf,...)                          LOG_ATLC( LogLevelInfo, LogComponentGeneric, "[CPU:%02x]"##buf, CpuGetApicId(), __VA_ARGS__ )
// #define LOGTPL(buf,...)                         LOGPL("[TH:%s]"##buf, ThreadGetName(NULL), __VA_ARGS__)

// #define LOG_TRACE(buf,...)                      LOG_ATLC( LogLevelTrace, LogComponentGeneric, "[CPU:%02x]"##buf, CpuGetApicId(), __VA_ARGS__ )
// #define LOG_TRACE_COMP(comp,buf,...)            LOG_ATLC( LogLevelTrace, (comp), "[CPU:%02x][" #comp "]" ##buf, CpuGetApicId(), __VA_ARGS__ )

// #define LOG_TRACE_IO(buf,...)                   LOG_TRACE_COMP(LogComponentIo, buf, __VA_ARGS__ )
// #define LOG_TRACE_EXCEPTION(buf,...)            LOG_TRACE_COMP(LogComponentException, buf, __VA_ARGS__ )
// #define LOG_TRACE_INTERRUPT(buf,...)            LOG_TRACE_COMP(LogComponentInterrupt, buf, __VA_ARGS__ )
// #define LOG_TRACE_VMM(buf,...)                  LOG_TRACE_COMP(LogComponentVmm, buf, __VA_ARGS__ )
// #define LOG_TRACE_MMU(buf,...)                  LOG_TRACE_COMP(LogComponentMmu, buf, __VA_ARGS__ )
// #define LOG_TRACE_CPU(buf,...)                  LOG_TRACE_COMP(LogComponentCpu, buf, __VA_ARGS__ )
// #define LOG_TRACE_ACPI(buf,...)                 LOG_TRACE_COMP(LogComponentAcpi, buf, __VA_ARGS__ )
// #define LOG_TRACE_THREAD(buf,...)               LOG_TRACE_COMP(LogComponentThread, buf, __VA_ARGS__ )
// #define LOG_TRACE_STORAGE(buf,...)              LOG_TRACE_COMP(LogComponentStorage, buf, __VA_ARGS__ )
// #define LOG_TRACE_FILESYSTEM(buf,...)           LOG_TRACE_COMP(LogComponentFileSystem, buf, __VA_ARGS__ )
// #define LOG_TRACE_NETWORK(buf,...)              LOG_TRACE_COMP(LogComponentNetwork, buf, __VA_ARGS__ )
// #define LOG_TRACE_USERMODE(buf,...)             LOG_TRACE_COMP(LogComponentUserMode, buf, __VA_ARGS__ )
// #define LOG_TRACE_PROCESS(buf,...)              LOG_TRACE_COMP(LogComponentProcess, buf, __VA_ARGS__)
// #define LOG_TRACE_PCI(buf,...)                  LOG_TRACE_COMP(LogComponentPci, buf, __VA_ARGS__)

// #define LOG_WARNING(buf,...)                    LOG_ATLC( LogLevelWarning, LogComponentGeneric, buf, __VA_ARGS__ )

// #define LOG_ERROR(buf,...)                      LOG_ATLC( LogLevelError, LogComponentGeneric, buf, __VA_ARGS__ )
// #define LOG_FUNC_ERROR(func,status)             LOG_ERROR("Function %s failed with status 0x%x\n", (func), (status) )
// #define LOG_FUNC_ERROR_ALLOC(func,size)         LOG_ERROR("Function %s failed alloc for size 0x%x\n", (func), (size))

// #define LOG_FUNC_START                          LOG_TRACE("Entering function %s\n", __FUNCTION__)
// #define LOG_FUNC_END                            LOG_TRACE("Leaving function %s\n", __FUNCTION__)

// #define LOG_FUNC_START_CPU                      LOG_FUNC_START
// #define LOG_FUNC_END_CPU                        LOG_FUNC_END

// #define LOG_FUNC_START_THREAD                   LOG_TRACE("[TH:%s]Entering function %s\n", ThreadGetName(NULL), __FUNCTION__)
// #define LOG_FUNC_END_THREAD                     LOG_TRACE("[TH:%s]Leaving function %s\n", ThreadGetName(NULL), __FUNCTION__)


// GCC has problems with __VA_ARGS__:
// 1. ", __VA_ARGS__" - if args is empty the comma is not removed, fix is ", ##__VA_ARGS__" (from C++20 we have __VA_OPT__())
// 2. if you use the fix GCC cannot process __VA_ARGS__ recursively, it may be a bug in the compiler idk
// => I had to rewrite them, so much fun...

#ifdef CAL_MSVC
#define SEPARATOR '\\'
#else
#define SEPARATOR '/'
#endif

#define LOG_ATC(lvl,comp,buf,...)               LogEx((lvl), (comp), (buf) VA_ARGS(__VA_ARGS__))

#define LOG_AT(lvl,buf,...)                     LogEx((lvl), LogComponentGeneric, buf VA_ARGS(__VA_ARGS__))
#define LOG_ATLC(lvl,comp,buf,...)              LogEx((lvl), (comp), "[%s][%d]" buf, strrchr(__FILE__, SEPARATOR) + 1, __LINE__ VA_ARGS(__VA_ARGS__))

#define LOG(buf,...)                            LogEx(LogLevelInfo, LogComponentGeneric, buf VA_ARGS(__VA_ARGS__))
#define LOGP(buf,...)                           LogEx(LogLevelInfo, LogComponentGeneric, "[CPU:%02x]" buf, CpuGetApicId() VA_ARGS(__VA_ARGS__))

#define LOGP_WARNING(buf,...)                   LogEx(LogLevelWarning, LogComponentGeneric, "[CPU:%02x]" buf, CpuGetApicId() VA_ARGS(__VA_ARGS__))
#define LOGP_ERROR(buf,...)                     LogEx(LogLevelError, LogComponentGeneric, "[CPU:%02x]" buf, CpuGetApicId() VA_ARGS(__VA_ARGS__))

#define LOGL(buf,...)                           LogEx(LogLevelInfo, LogComponentGeneric, "[%s][%d]" buf, strrchr(__FILE__, SEPARATOR) + 1, __LINE__ VA_ARGS(__VA_ARGS__))
#define LOGPL(buf,...)                          LogEx(LogLevelInfo, LogComponentGeneric, "[CPU:%02x][%s][%d]" buf, CpuGetApicId(), strrchr(__FILE__,SEPARATOR) + 1, __LINE__ VA_ARGS(__VA_ARGS__))
#define LOGTPL(buf,...)                         LogEx(LogLevelInfo, LogComponentGeneric, "[TH:%s][CPU:%02x][%s][%d]" buf, ThreadGetName(NULL), CpuGetApicId(), strrchr(__FILE__,'\\') + 1, __LINE__ VA_ARGS(__VA_ARGS__))

#define LOG_TRACE(buf,...)                      LogEx(LogLevelTrace, LogComponentGeneric, "[CPU:%02x][%s][%d]" buf, CpuGetApicId(), strrchr(__FILE__, SEPARATOR) + 1, __LINE__ VA_ARGS(__VA_ARGS__))
#define LOG_TRACE_COMP(comp,buf,...)            LogEx(LogLevelTrace, (comp), "[CPU:%02x][" #comp "][%s][%d]" buf, CpuGetApicId(), strrchr(__FILE__, SEPARATOR) + 1, __LINE__ VA_ARGS(__VA_ARGS__))

#define LOG_TRACE_IO(buf,...)                   LogEx(LogLevelTrace, LogComponentIo, "[CPU:%02x][LogComponentIo]][%s][%d]" buf, CpuGetApicId(), strrchr(__FILE__, SEPARATOR) + 1, __LINE__ VA_ARGS(__VA_ARGS__))
#define LOG_TRACE_EXCEPTION(buf,...)            LogEx(LogLevelTrace, LogComponentException, "[CPU:%02x][LogComponentException]][%s][%d]" buf, CpuGetApicId(), strrchr(__FILE__,SEPARATOR) + 1, __LINE__ VA_ARGS(__VA_ARGS__))
#define LOG_TRACE_INTERRUPT(buf,...)            LogEx(LogLevelTrace, LogComponentInterrupt, "[CPU:%02x][LogComponentInterrupt]][%s][%d]" buf, CpuGetApicId(), strrchr(__FILE__,SEPARATOR) + 1, __LINE__ VA_ARGS(__VA_ARGS__))
#define LOG_TRACE_VMM(buf,...)                  LogEx(LogLevelTrace, LogComponentVmm, "[CPU:%02x][LogComponentVmm]][%s][%d]" buf, CpuGetApicId(), strrchr(__FILE__, SEPARATOR) + 1, __LINE__ VA_ARGS(__VA_ARGS__))
#define LOG_TRACE_MMU(buf,...)                  LogEx(LogLevelTrace, LogComponentMmu, "[CPU:%02x][LogComponentMmu]][%s][%d]" buf, CpuGetApicId(), strrchr(__FILE__,SEPARATOR) + 1, __LINE__ VA_ARGS(__VA_ARGS__))
#define LOG_TRACE_CPU(buf,...)                  LogEx(LogLevelTrace, LogComponentCpu, "[CPU:%02x][LogComponentCpu]][%s][%d]" buf, CpuGetApicId(), strrchr(__FILE__,SEPARATOR) + 1, __LINE__ VA_ARGS(__VA_ARGS__))
#define LOG_TRACE_ACPI(buf,...)                 LogEx(LogLevelTrace, LogComponentAcpi, "[CPU:%02x][LogComponentAcpi]][%s][%d]" buf, CpuGetApicId(), strrchr(__FILE__,SEPARATOR) + 1, __LINE__ VA_ARGS(__VA_ARGS__))
#define LOG_TRACE_THREAD(buf,...)               LogEx(LogLevelTrace, LogComponentThread, "[CPU:%02x][LogComponentThread]][%s][%d]" buf, CpuGetApicId(), strrchr(__FILE__,SEPARATOR) + 1, __LINE__ VA_ARGS(__VA_ARGS__))
#define LOG_TRACE_STORAGE(buf,...)              LogEx(LogLevelTrace, LogComponentStorage, "[CPU:%02x][LogComponentStorage]][%s][%d]" buf, CpuGetApicId(), strrchr(__FILE__,SEPARATOR) + 1, __LINE__ VA_ARGS(__VA_ARGS__))
#define LOG_TRACE_FILESYSTEM(buf,...)           LogEx(LogLevelTrace, LogComponentFileSystem, "[CPU:%02x][LogComponentFileSystem]][%s][%d]" buf, CpuGetApicId(), strrchr(__FILE__,SEPARATOR) + 1, __LINE__ VA_ARGS(__VA_ARGS__))
#define LOG_TRACE_NETWORK(buf,...)              LogEx(LogLevelTrace, LogComponentNetwork, "[CPU:%02x][LogComponentNetwork]][%s][%d]" buf, CpuGetApicId(), strrchr(__FILE__,SEPARATOR) + 1, __LINE__ VA_ARGS(__VA_ARGS__))
#define LOG_TRACE_USERMODE(buf,...)             LogEx(LogLevelTrace, LogComponentUserMode, "[CPU:%02x][LogComponentUserMode]][%s][%d]" buf, CpuGetApicId(), strrchr(__FILE__,SEPARATOR) + 1, __LINE__ VA_ARGS(__VA_ARGS__))
#define LOG_TRACE_PROCESS(buf,...)              LogEx(LogLevelTrace, LogComponentProcess, "[CPU:%02x][LogComponentProcess]][%s][%d]" buf, CpuGetApicId(), strrchr(__FILE__,SEPARATOR) + 1, __LINE__ VA_ARGS(__VA_ARGS__))
#define LOG_TRACE_PCI(buf,...)                  LogEx(LogLevelTrace, LogComponentPci, "[CPU:%02x][LogComponentPci]][%s][%d]" buf, CpuGetApicId(), strrchr(__FILE__,SEPARATOR) + 1, __LINE__ VA_ARGS(__VA_ARGS__))

#define LOG_WARNING(buf,...)                    LogEx(LogLevelWarning, LogComponentGeneric, "[%s][%d]" buf, strrchr(__FILE__,SEPARATOR) + 1, __LINE__ VA_ARGS(__VA_ARGS__))

#define LOG_ERROR(buf,...)                      LogEx(LogLevelError, LogComponentGeneric, "[%s][%d]" buf, strrchr(__FILE__,SEPARATOR) + 1, __LINE__ VA_ARGS(__VA_ARGS__))
#define LOG_FUNC_ERROR(func,status)             LogEx(LogLevelError, LogComponentGeneric, "[%s][%d]Function %s failed with status 0x%x\n", strrchr(__FILE__,SEPARATOR) + 1, __LINE__, func, status)
#define LOG_FUNC_ERROR_ALLOC(func,size)         LogEx(LogLevelError, LogComponentGeneric, "[%s][%d]Function %s failed alloc for size 0x%x\n", strrchr(__FILE__,SEPARATOR) + 1, __LINE__, func, size)

#define LOG_FUNC_START                          LogEx(LogLevelTrace, LogComponentGeneric, "[CPU:%02x][%s][%d]Entering function %s\n", CpuGetApicId(), strrchr(__FILE__,SEPARATOR) + 1, __LINE__, __FUNCTION__)
#define LOG_FUNC_END                            LogEx(LogLevelTrace, LogComponentGeneric, "[CPU:%02x][%s][%d]Leaving function %s\n", CpuGetApicId(), strrchr(__FILE__,SEPARATOR) + 1, __LINE__, __FUNCTION__)

#define LOG_FUNC_START_CPU                      LOG_FUNC_START
#define LOG_FUNC_END_CPU                        LOG_FUNC_END

#define LOG_FUNC_START_THREAD                   LogEx(LogLevelTrace, LogComponentGeneric, "[TH:%s][CPU:%02x][%s][%d]Entering function %s\n", ThreadGetName(NULL), CpuGetApicId(), strrchr(__FILE__,SEPARATOR) + 1, __LINE__, __FUNCTION__)
#define LOG_FUNC_END_THREAD                     LogEx(LogLevelTrace, LogComponentGeneric, "[TH:%s][CPU:%02x][%s][%d]Leaving function %s\n", ThreadGetName(NULL), CpuGetApicId(), strrchr(__FILE__,SEPARATOR) + 1, __LINE__, __FUNCTION__)

#else
#define LOG_AT(lvl,buf,...)
#define LOG_ATL(lvl,buf,...)

#define LOG(buf,...)
#define LOGP(buf,...)
#define LOGP_ERROR(buf,...)

#define LOGL(buf,...)
#define LOGPL(buf,...)
#define LOGTPL(buf,...)

#define LOG_TRACE(buf,...)
#define LOG_TRACE_COMP(comp,buf,...)

#define LOG_TRACE_IO(buf,...)
#define LOG_TRACE_EXCEPTION(buf,...)
#define LOG_TRACE_INTERRUPT(buf,...)
#define LOG_TRACE_VMM(buf,...)
#define LOG_TRACE_MMU(buf,...)
#define LOG_TRACE_CPU(buf,...)
#define LOG_TRACE_ACPI(buf,...)
#define LOG_TRACE_THREAD(buf,...)
#define LOG_TRACE_STORAGE(buf,...)
#define LOG_TRACE_FILESYSTEM(buf,...)
#define LOG_TRACE_NETWORK(buf,...)
#define LOG_TRACE_USERMODE(buf,...)
#define LOG_TRACE_PROCESS(buf,...)
#define LOG_TRACE_PCI(buf,...)

#define LOG_WARNING(buf,...)

#define LOG_ERROR(buf,...)
#define LOG_FUNC_ERROR(func,status)
#define LOG_FUNC_ERROR_ALLOC(func,size)

#define LOG_FUNC_START
#define LOG_FUNC_END

#define LOG_FUNC_START_CPU
#define LOG_FUNC_END_CPU

#define LOG_FUNC_START_THREAD
#define LOG_FUNC_END_THREAD

#endif

typedef enum _LOG_LEVEL
{
    LogLevelTrace,
    LogLevelInfo,
    LogLevelWarning,
    LogLevelError
} LOG_LEVEL;
STATIC_ASSERT_INFO(sizeof(LOG_LEVEL) == sizeof(DWORD), "We are using AtomicExchange32 for levels!");

typedef enum _LOG_COMPONENT
{
    LogComponentGeneric     = 0b00000000000000000000000000000001,
    LogComponentIo          = 0b00000000000000000000000000000010,
    LogComponentException   = 0b00000000000000000000000000000100,
    LogComponentInterrupt   = 0b00000000000000000000000000001000,
    LogComponentVmm         = 0b00000000000000000000000000010000,
    LogComponentMmu         = 0b00000000000000000000000000100000,
    LogComponentCpu         = 0b00000000000000000000000001000000,
    LogComponentAcpi        = 0b00000000000000000000000010000000,
    LogComponentThread      = 0b00000000000000000000000100000000,
    LogComponentStorage     = 0b00000000000000000000001000000000,
    LogComponentFileSystem  = 0b00000000000000000000010000000000,
    LogComponentNetwork     = 0b00000000000000000000100000000000,
    LogComponentUserMode    = 0b00000000000000000001000000000000,
    LogComponentProcess     = 0b00000000000000000010000000000000,
    LogComponentPci         = 0b00000000000000000100000000000000,
    LogComponentTest        = 0b00000000000000001000000000000000,

    LogComponentAll         = 0b11111111111111111111111111111111
} _Enum_is_bitflag_ LOG_COMPONENT;
STATIC_ASSERT_INFO(sizeof(LOG_COMPONENT) == sizeof(DWORD), "We are using AtomicExchange32 for components!");

_No_competing_thread_
void
LogSystemPreinit(
    void
    );

_No_competing_thread_
void
LogSystemInit(
    IN _Strict_type_match_
                LOG_LEVEL       LogLevel,
    IN
                LOG_COMPONENT   LogComponenets,
    IN          BOOLEAN         Enable
    );

#define Log(lvl,buf,...)        LogEx((lvl),LogComponentGeneric,(buf), __VA_ARGS__)

void
LogEx(
    IN _Strict_type_match_
                LOG_LEVEL       LogLevel,
    IN
                LOG_COMPONENT   LogComponent,
    IN_Z        char*           FormatBuffer,
    ...
    );

BOOLEAN
LogSetState(
    IN          BOOLEAN     Enable
    );

LOG_LEVEL
LogGetLevel(
    void
    );

LOG_LEVEL
LogSetLevel(
    IN          LOG_LEVEL   NewLogLevel
    );

LOG_COMPONENT
LogGetTracedComponents(
    void
    );

LOG_COMPONENT
LogSetTracedComponents(
    IN          LOG_COMPONENT   Components
    );

#define LogIsComponentTraced(Comp)      (LogGetLevel() <= LogLevelTrace && IsFlagOn(LogGetTracedComponents(), (Comp)))
