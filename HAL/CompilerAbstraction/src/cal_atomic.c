#include "cal_atomic.h"

uint8_t AtomicCompareExchange8(uint8_t volatile *destination, uint8_t exchange, uint8_t comparand)
{
    uint8_t expected;
    BOOLEAN result;

    expected = comparand;
    result = __atomic_compare_exchange_n(destination,
                                         &expected,
                                         exchange,
                                         0,
                                         __ATOMIC_SEQ_CST,
                                         __ATOMIC_SEQ_CST);
    
    if (result)
    {
        return comparand;
    }

    return expected;
}

uint16_t AtomicCompareExchange16(uint16_t volatile *destination, uint16_t exchange, uint16_t comparand)
{
    uint16_t expected;
    BOOLEAN result;

    expected = comparand;
    result = __atomic_compare_exchange_n(destination,
                                         &expected,
                                         exchange,
                                         0,
                                         __ATOMIC_SEQ_CST,
                                         __ATOMIC_SEQ_CST);
    
    if (result)
    {
        return comparand;
    }

    return expected;
}

uint32_t AtomicCompareExchange32(uint32_t volatile *destination, uint32_t exchange, uint32_t comparand)
{
    uint32_t expected;
    BOOLEAN result;

    expected = comparand;
    result = __atomic_compare_exchange_n(destination,
                                         &expected,
                                         exchange,
                                         0,
                                         __ATOMIC_SEQ_CST,
                                         __ATOMIC_SEQ_CST);
    
    if (result)
    {
        return comparand;
    }

    return expected;
}

uint64_t AtomicCompareExchange64(uint64_t volatile *destination, uint64_t exchange, uint64_t comparand)
{
    uint64_t expected;
    BOOLEAN result;

    expected = comparand;
    result = __atomic_compare_exchange_n(destination,
                                         &expected,
                                         exchange,
                                         0,
                                         __ATOMIC_SEQ_CST,
                                         __ATOMIC_SEQ_CST);
    
    if (result)
    {
        return comparand;
    }

    return expected;
}

void* AtomicCompareExchangePointer(void * volatile * destination, void* exchange, void* comparand)
{
    void* expected;
    BOOLEAN result;

    expected = comparand;
    result = __atomic_compare_exchange_n(destination,
                                         &expected,
                                         exchange,
                                         0,
                                         __ATOMIC_SEQ_CST,
                                         __ATOMIC_SEQ_CST);
    
    if (result)
    {
        return comparand;
    }

    return expected;
}
