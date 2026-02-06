#include <Bootloader/acpi.h>

#include <Guid/Acpi.h>
#include <Bootloader/memory.h>

UINT64
GetAcpiRsdp(
    EFI_SYSTEM_TABLE *ST
    )
{
    UINT64 count = ST->NumberOfTableEntries;
    EFI_CONFIGURATION_TABLE *table = ST->ConfigurationTable;
    EFI_GUID acpiGuid = EFI_ACPI_TABLE_GUID;

    for (UINT64 i = 0; i < count; i++)
        if (MemoryEquals(&acpiGuid, &(table[i].VendorGuid), sizeof(EFI_GUID)))
            return (UINT64) table[i].VendorTable;
    
    return (UINT64) NULL;
}