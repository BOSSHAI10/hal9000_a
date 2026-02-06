#pragma once

typedef enum _APIC_DELIVERY_MODE
{
    ApicDeliveryModeFixed,
    ApicDeliveryModeLowest,
    ApicDeliveryModeSMI = 2,
    ApicDeliveryModeNMI = 4,
    ApicDeliveryModeINIT,
    ApicDeliveryModeSIPI,
    ApicDeliveryModeExtINT
} APIC_DELIVERY_MODE;

typedef enum _APIC_DESTINATION_MODE
{
    ApicDestinationModePhysical,
    ApicDestinationModeLogical
} APIC_DESTINATION_MODE;

typedef enum _APIC_DESTINATION_SHORTHAND
{
    ApicDestinationShorthandNone,
    ApicDestinationShorthandSelf,
    ApicDestinationShorthandAll,
    ApicDestinationShorthandAllExcludingSelf
} APIC_DESTINATION_SHORTHAND;

typedef enum _APIC_DIVIDE_VALUE
{
    ApicDivideBy2       = 0b0000,
    ApicDivideBy4       = 0b0001,
    ApicDivideBy8       = 0b0010,
    ApicDivideBy16      = 0b0011,
    ApicDivideBy32      = 0b1000,
    ApicDivideBy64      = 0b1001,
    ApicDivideBy128     = 0b1010,
    ApicDivideBy1       = 0b1011,

    ApicDivideReserved  = 0b1111
} APIC_DIVIDE_VALUE;

typedef enum _APIC_PIN_POLARITY
{
    ApicPinPolarityActiveHigh,
    ApicPinPolarityActiveLow
} APIC_PIN_POLARITY;

typedef enum _APIC_TRIGGER_MODE
{
    ApicTriggerModeEdge,
    ApicTriggerModeLevel
} APIC_TRIGGER_MODE;

#define APIC_DESTINATION_FORMAT_FLAT_MODEL         0b1111
#define APIC_DESTINATION_FORMAT_CLUSTER_MODEL      0b0000