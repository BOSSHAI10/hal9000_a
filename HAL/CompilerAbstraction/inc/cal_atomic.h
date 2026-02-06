#pragma once

#include "cal_compiler.h"
#include "cal_types.h"

#ifdef CAL_MSVC

#define AtomicAnd8(value, mask) _InterlockedAnd8((int8_t volatile *) (value), (int8_t) (mask))
#define AtomicAnd16(value, mask) _InterlockedAnd16((int16_t volatile *) (value), (int16_t) (mask))
#define AtomicAnd32(value, mask) _InterlockedAnd((int32_t volatile *) (value), (int32_t) (mask))
#define AtomicAnd64(value, mask) _InterlockedAnd64((int64_t volatile *) (value), (int64_t) (mask))

#define AtomicCompareExchange8(destination, exchange, comparand) _InterlockedCompareExchange8((int8_t volatile *) (destination), (int8_t) (exchange), (int8_t) (comparand))
#define AtomicCompareExchange16(destination, exchange, comparand) _InterlockedCompareExchange16((int16_t volatile *) (destination), (int16_t) (exchange), (int16_t) (comparand))
#define AtomicCompareExchange32(destination, exchange, comparand) _InterlockedCompareExchange((int32_t volatile *) (destination), (int32_t) (exchange), (int32_t) (comparand))
#define AtomicCompareExchange64(destination, exchange, comparand) _InterlockedCompareExchange64((int64_t volatile *) (destination), (int64_t) (exchange), (int64_t) (comparand))

#define AtomicCompareExchangePointer(destination, exchange, comparand) _InterlockedCompareExchangePointer((void * volatile *) (destination), (void *) (exchange), (void *) (comparand))

#define AtomicDecrement16(ptr) _InterlockedDecrement16((int16_t volatile *) (ptr))
#define AtomicDecrement32(ptr) _InterlockedDecrement((int32_t volatile *) (ptr))
#define AtomicDecrement64(ptr) _InterlockedDecrement64((int64_t volatile *) (ptr))

#define AtomicExchange8(target, value) _InterlockedExchange8((int8_t volatile *) (target), (int8_t) (value))
#define AtomicExchange16(target, value) _InterlockedExchange16((int16_t volatile *) (target), (int16_t) (value))
#define AtomicExchange32(target, value) _InterlockedExchange((int32_t volatile *) (target), (int32_t) (value))
#define AtomicExchange64(target, value) _InterlockedExchange64((int64_t volatile *) (target), (int64_t) (value))

#define AtomicExchangeAdd8(addend, value) _InterlockedExchangeAdd8((int8_t volatile *) (addend), (int8_t) (value))
#define AtomicExchangeAdd16(addend, value) _InterlockedExchangeAdd16((int16_t volatile *) (addend), (int16_t) (value))
#define AtomicExchangeAdd32(addend, value) _InterlockedExchangeAdd((int32_t volatile *) (addend), (int32_t) (value))
#define AtomicExchangeAdd64(addend, value) _InterlockedExchangeAdd64((int64_t volatile *) (addend), (int64_t) (value))

#define AtomicExchangePointer(target, value) _InterlockedExchangePointer((void * volatile *) (target), (void *) (value))

#define AtomicIncrement16(ptr) _InterlockedIncrement16((int16_t volatile *) (ptr))
#define AtomicIncrement32(ptr) _InterlockedIncrement((int32_t volatile *) (ptr))
#define AtomicIncrement64(ptr) _InterlockedIncrement64((int64_t volatile *) (ptr))

#define AtomicOr8(value, mask) _InterlockedOr8((int8_t volatile *) (value), (int8_t) (mask))
#define AtomicOr16(value, mask) _InterlockedOr16((int16_t volatile *) (value), (int16_t) (mask))
#define AtomicOr32(value, mask) _InterlockedOr((int32_t volatile *) (value), (int32_t) (mask))
#define AtomicOr64(value, mask) _InterlockedOr64((int64_t volatile *) (value), (int64_t) (mask))

#define AtomicXor8(value, mask) _InterlockedXor8((int8_t volatile *) (value), (int8_t) (mask)) 
#define AtomicXor16(value, mask) _InterlockedXor16((int16_t volatile *) (value), (int16_t) (mask)) 
#define AtomicXor32(value, mask) _InterlockedXor((int32_t volatile *) (value), (int32_t) (mask)) 
#define AtomicXor64(value, mask) _InterlockedXor64((int64_t volatile *) (value), (int64_t) (mask)) 

#define AtomicReadWriteBarrier _ReadWriteBarrier

#else

#define AtomicAnd8(value, mask) __atomic_fetch_and((int8_t volatile *) (value), (int8_t) (mask), __ATOMIC_SEQ_CST)
#define AtomicAnd16(value, mask) __atomic_fetch_and((int16_t volatile *) (value), (int16_t) (mask), __ATOMIC_SEQ_CST)
#define AtomicAnd32(value, mask) __atomic_fetch_and((int32_t volatile *) (value), (int32_t) (mask), __ATOMIC_SEQ_CST)
#define AtomicAnd64(value, mask) __atomic_fetch_and((int64_t volatile *) (value), (int64_t) (mask), __ATOMIC_SEQ_CST)

uint8_t AtomicCompareExchange8(uint8_t volatile *destination, uint8_t exchange, uint8_t comparand);
uint16_t AtomicCompareExchange16(uint16_t volatile *destination, uint16_t exchange, uint16_t comparand);
uint32_t AtomicCompareExchange32(uint32_t volatile *destination, uint32_t exchange, uint32_t comparand);
uint64_t AtomicCompareExchange64(uint64_t volatile *destination, uint64_t exchange, uint64_t comparand);

void* AtomicCompareExchangePointer(void * volatile * destination, void* exchange, void* comparand);

#define AtomicDecrement16(ptr) __atomic_add_fetch((int16_t volatile *) (ptr), (int16_t) -1, __ATOMIC_SEQ_CST)
#define AtomicDecrement32(ptr) __atomic_add_fetch((int32_t volatile *) (ptr), (int32_t) -1, __ATOMIC_SEQ_CST)
#define AtomicDecrement64(ptr) __atomic_add_fetch((int64_t volatile *) (ptr), (int64_t) -1, __ATOMIC_SEQ_CST)

#define AtomicExchange8(target, value) __atomic_exchange_n((int8_t volatile *) (target), (int8_t) value, __ATOMIC_SEQ_CST)
#define AtomicExchange16(target, value) __atomic_exchange_n((int16_t volatile *) (target), (int16_t) value, __ATOMIC_SEQ_CST)
#define AtomicExchange32(target, value) __atomic_exchange_n((int32_t volatile *) (target), (int32_t) value, __ATOMIC_SEQ_CST)
#define AtomicExchange64(target, value) __atomic_exchange_n((int64_t volatile *) (target), (int64_t) value, __ATOMIC_SEQ_CST)

#define AtomicExchangeAdd8(ptr, val) __atomic_fetch_add((int8_t volatile *) (ptr), (int8_t) (val), __ATOMIC_SEQ_CST)
#define AtomicExchangeAdd16(ptr, val) __atomic_fetch_add((int16_t volatile *) (ptr), (int16_t) (val), __ATOMIC_SEQ_CST)
#define AtomicExchangeAdd32(ptr, val) __atomic_fetch_add((int32_t volatile *) (ptr), (int32_t) (val), __ATOMIC_SEQ_CST)
#define AtomicExchangeAdd64(ptr, val) __atomic_fetch_add((int64_t volatile *) (ptr), (int64_t) (val), __ATOMIC_SEQ_CST)

#define AtomicExchangePointer(target, value) __atomic_exchange_n((void * volatile *) (target), (void *) (value), __ATOMIC_SEQ_CST)

#define AtomicIncrement16(ptr) __atomic_add_fetch((int16_t volatile *) (ptr), (int16_t) 1, __ATOMIC_SEQ_CST)
#define AtomicIncrement32(ptr) __atomic_add_fetch((int32_t volatile *) (ptr), (int32_t) 1, __ATOMIC_SEQ_CST)
#define AtomicIncrement64(ptr) __atomic_add_fetch((int64_t volatile *) (ptr), (int64_t) 1, __ATOMIC_SEQ_CST)

#define AtomicOr8(value, mask) __atomic_fetch_or((int8_t volatile *) (value), (int8_t) (mask), __ATOMIC_SEQ_CST)
#define AtomicOr16(value, mask) __atomic_fetch_or((int16_t volatile *) (value), (int16_t) (mask), __ATOMIC_SEQ_CST)
#define AtomicOr32(value, mask) __atomic_fetch_or((int32_t volatile *) (value), (int32_t) (mask), __ATOMIC_SEQ_CST)
#define AtomicOr64(value, mask) __atomic_fetch_or((int64_t volatile *) (value), (int64_t) (mask), __ATOMIC_SEQ_CST)

#define AtomicXor8(value, mask) __atomic_fetch_or((int8_t volatile *) (value), (int8_t) (mask), __ATOMIC_SEQ_CST)
#define AtomicXor16(value, mask) __atomic_fetch_or((int16_t volatile *) (value), (int16_t) (mask), __ATOMIC_SEQ_CST)
#define AtomicXor32(value, mask) __atomic_fetch_or((int32_t volatile *) (value), (int32_t) (mask), __ATOMIC_SEQ_CST)
#define AtomicXor64(value, mask) __atomic_fetch_or((int64_t volatile *) (value), (int64_t) (mask), __ATOMIC_SEQ_CST)

#define AtomicReadWriteBarrier __sync_synchronize

#endif
