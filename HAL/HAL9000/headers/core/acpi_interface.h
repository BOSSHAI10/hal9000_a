#pragma once

#include "cal_warning.h"

WARNING_PUSH
GNU_WARNING_IGNORE("-Wunused-parameter")
#include "acpi.h"
WARNING_POP

void
AcpiInterfacePreinit(
    void
    );

STATUS
AcpiInterfaceInit(
    void
    );

void
AcpiShutdown(
    void
    );

STATUS
AcpiInterfaceLateInit(
    void
    );

STATUS
AcpiRetrieveNextCpu(
    IN      BOOLEAN                     RestartSearch,
    OUT_PTR ACPI_MADT_LOCAL_APIC**      AcpiEntry
    );

STATUS
AcpiRetrieveNextIoApic(
    IN      BOOLEAN                     RestartSearch,
    OUT_PTR ACPI_MADT_IO_APIC**         AcpiEntry
    );

STATUS
AcpiRetrieveNextInterruptOverride(
    IN      BOOLEAN                         RestartSearch,
    OUT_PTR ACPI_MADT_INTERRUPT_OVERRIDE**  AcpiEntry
    );

STATUS
AcpiRetrieveNextMcfgEntry(
    IN      BOOLEAN                     RestartSearch,
    OUT_PTR ACPI_MCFG_ALLOCATION**      AcpiEntry
    );

STATUS
AcpiRetrieveNextPrtEntry(
    IN      BOOLEAN                     RestartSearch,
    OUT_PTR ACPI_PCI_ROUTING_TABLE**    AcpiEntry,
    OUT     BYTE*                       BusNumber,
    OUT     WORD*                       SegmentNumber
    );