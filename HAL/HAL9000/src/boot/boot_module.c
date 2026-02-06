#include "HAL9000.h"
#include "boot_module.h"
#include "mmu.h"
#include "bootinfo.h"
#include "cal_seh.h"
#include "cal_warning.h"

#define BOOT_MODULE_MAX_NAME_LEN        33

typedef struct _BOOT_MODULE_INFORMATION
{
    PVOID                           BaseAddress;
    QWORD                           Length;

    char*                           Name;
} BOOT_MODULE_INFORMATION, *PBOOT_MODULE_INFORMATION;

typedef struct _BOOT_MODULE_SYSTEM_DATA
{
    PBOOT_MODULE_INFORMATION        Modules;

    DWORD                           NumberOfModules;
} BOOT_MODULE_SYSTEM_DATA, *PBOOT_MODULE_SYSTEM_DATA;

static BOOT_MODULE_SYSTEM_DATA m_bootModuleInformation;

static
STATUS
_MapSingleModule(
    OUT     PBOOT_MODULE_INFORMATION            MappedModule,
    IN      HAL_BOOT_MODULE                     *BootModule
    );

static
void
_BootModuleUninitialize(
    _Pre_valid_ _Post_invalid_
            PBOOT_MODULE_INFORMATION            BootModule
    );

static
void
_BootModulesUninitialize(
    _Pre_valid_ _Post_ptr_invalid_
            PBOOT_MODULE_INFORMATION            BootModules,
    IN      DWORD                               NumberOfModules
    );

_No_competing_thread_
void
BootModulesPreinit(
    void
    )
{
    memzero(&m_bootModuleInformation, sizeof(BOOT_MODULE_SYSTEM_DATA));
}

_No_competing_thread_
STATUS
BootModulesInit(
    IN      PHYSICAL_ADDRESS        BootModulesStart,
    IN      DWORD                   NumberOfModules
    )
{
    STATUS status;
    PBOOT_MODULE_INFORMATION pModuleInformation;
    HAL_BOOT_MODULE *pBootModules;

    if (NumberOfModules == 0)
    {
        // nothing to map
        return STATUS_SUCCESS;
    }

    if (BootModulesStart == NULL)
    {
        return STATUS_INVALID_PARAMETER1;
    }

    status = STATUS_SUCCESS;
    pModuleInformation = NULL;
    pBootModules = NULL;

    __try
    {
        pModuleInformation = ExAllocatePoolWithTag(PoolAllocateZeroMemory,
                                                   NumberOfModules * sizeof(BOOT_MODULE_INFORMATION),
                                                   HEAP_BOOT_TAG,
                                                   0);
        if (pModuleInformation == NULL)
        {
            status = STATUS_HEAP_INSUFFICIENT_RESOURCES;
            LOG_FUNC_ERROR_ALLOC("ExAllocatePoolWithTag", NumberOfModules * sizeof(BOOT_MODULE_INFORMATION));
            __leave;
        }

        pBootModules = MmuMapSystemMemory(BootModulesStart, NumberOfModules * sizeof(HAL_BOOT_MODULE));
        LOG("Boot module vector mapped\n");
        if (pBootModules == NULL)
        {
            status = STATUS_MEMORY_CANNOT_BE_MAPPED;
            LOG_FUNC_ERROR_ALLOC("MmuMapSystemMemory",NumberOfModules * sizeof(HAL_BOOT_MODULE));
            __leave;
        }

        for (DWORD i = 0; i < NumberOfModules; ++i)
        {
            status = _MapSingleModule(&pModuleInformation[i], &pBootModules[i]);
            if (!SUCCEEDED(status))
            {
                LOG_WARNING("_MapSingleModule failed with status 0x%x\n", status);

                // This component (boot_module.c) doesn't know which modules are essential and which are not
                // so we do our best effort to map as many modules as possible even if some fail
                // Some component higher than us in the hierarchy will decide which components are mandatory and which
                // are not
                status = STATUS_SUCCESS;
                continue;
            }
        }
    }
    __finally
    {
        if (pBootModules != NULL)
        {
            MmuUnmapSystemMemory(pBootModules, NumberOfModules * sizeof(HAL_BOOT_MODULE));
            pBootModules = NULL;
        }

        if (!SUCCEEDED(status))
        {
            _BootModulesUninitialize(pModuleInformation, NumberOfModules);
            pModuleInformation = NULL;
        }
        else
        {
            m_bootModuleInformation.NumberOfModules = NumberOfModules;
            m_bootModuleInformation.Modules = pModuleInformation;
        }
    }

    return status;
}

_No_competing_thread_
void
BootModulesUninit(
    void
    )
{
    if (m_bootModuleInformation.Modules != NULL)
    {
        _BootModulesUninitialize(m_bootModuleInformation.Modules,
                                 m_bootModuleInformation.NumberOfModules);
    }

    BootModulesPreinit();
}

STATUS
BootModuleGet(
    IN_Z    char*                   ModuleName,
    OUT     PVOID*                  BaseAddress,
    OUT     QWORD*                  ModuleLength
    )
{
    if (ModuleName == NULL)
    {
        return STATUS_INVALID_PARAMETER1;
    }

    if (BaseAddress == NULL)
    {
        return STATUS_INVALID_PARAMETER2;
    }

    if (ModuleLength == NULL)
    {
        return STATUS_INVALID_PARAMETER3;
    }

    for (DWORD i = 0; i < m_bootModuleInformation.NumberOfModules; ++i)
    {
        if (stricmp(ModuleName, m_bootModuleInformation.Modules[i].Name) == 0)
        {
            *BaseAddress = m_bootModuleInformation.Modules[i].BaseAddress;
            *ModuleLength = m_bootModuleInformation.Modules[i].Length;

            return STATUS_SUCCESS;
        }
    }

    return STATUS_ELEMENT_NOT_FOUND;
}

WARNING_PUSH
GNU_WARNING_IGNORE("-Wtautological-pointer-compare")

static
STATUS
_MapSingleModule(
    OUT     PBOOT_MODULE_INFORMATION            MappedModule,
    IN      HAL_BOOT_MODULE                     *BootModule
    )
{
    STATUS status;
    DWORD modNameLen;
    PVOID pModuleData;
    BOOT_MODULE_INFORMATION bootMod;

    ASSERT(MappedModule != NULL);
    ASSERT(BootModule != NULL);

    status = STATUS_SUCCESS;
    pModuleData = NULL;
    memzero(&bootMod, sizeof(BOOT_MODULE_INFORMATION));

    __try
    {
        // The string field may be 0 if there is no string associated with the module.
        if (BootModule->Name != 0)
        {
            LOG("Mapping module %s\n", BootModule->Name);

            // The string field provides an arbitrary string to be associated with that particular boot module;
            // it is a zero-terminated ASCII string
            modNameLen = strlen_s(BootModule->Name, BOOT_MODULE_MAX_NAME_LEN);
            ASSERT(modNameLen != INVALID_STRING_SIZE);

            bootMod.Name = ExAllocatePoolWithTag(0, modNameLen + 1, HEAP_BOOT_TAG, 0);
            if (bootMod.Name == NULL)
            {
                status = STATUS_HEAP_INSUFFICIENT_RESOURCES;
                LOG_FUNC_ERROR_ALLOC("ExAllocatePoolWithTag", modNameLen);
                __leave;
            }
            memcpy(bootMod.Name, BootModule->Name, modNameLen);
            bootMod.Name[modNameLen + 1] = '\0';
        }

        bootMod.Length = BootModule->Size;

        if (bootMod.Length == 0)
        {
            LOG_WARNING("There's nothing we can do with a module with size 0!\n");
            status = STATUS_NO_DATA_AVAILABLE;
            __leave;
        }

        LOG("Will try to map module between 0x%x -> 0x%x with name [%s] of size 0x%x\n",
            BootModule->PhysicalAddress, BootModule->PhysicalAddress + bootMod.Length,
            bootMod.Name, bootMod.Length);

        pModuleData = MmuMapSystemMemory((PHYSICAL_ADDRESS)(QWORD)BootModule->PhysicalAddress,
                                         bootMod.Length);
        if (pModuleData == NULL)
        {
            status = STATUS_MEMORY_CANNOT_BE_MAPPED;
            LOG_FUNC_ERROR_ALLOC("MmuMapSystemMemory", bootMod.Length);
            __leave;
        }
        ASSERT(bootMod.Length + 1 <= MAX_DWORD);

        bootMod.BaseAddress = ExAllocatePoolWithTag(0, (DWORD) bootMod.Length, HEAP_BOOT_TAG, 0);
        if (bootMod.BaseAddress == NULL)
        {
            status = STATUS_HEAP_INSUFFICIENT_RESOURCES;
            LOG_FUNC_ERROR_ALLOC("ExAllocatePoolWithTag", bootMod.Length);
            __leave;
        }

        memcpy(bootMod.BaseAddress, pModuleData, bootMod.Length);
    }
    __finally
    {
        if (pModuleData != NULL)
        {
            ASSERT(bootMod.Length != 0);

            MmuUnmapSystemMemory(pModuleData, bootMod.Length);
            pModuleData = NULL;
        }

        if (!SUCCEEDED(status))
        {
            _BootModuleUninitialize(&bootMod);
        }
        else
        {
            memcpy(MappedModule, &bootMod, sizeof(BOOT_MODULE_INFORMATION));
        }
    }

    return status;
}

WARNING_POP

static
void
_BootModuleUninitialize(
    _Pre_valid_ _Post_invalid_
            PBOOT_MODULE_INFORMATION            BootModule
    )
{
    ASSERT(BootModule != NULL);

    if (BootModule->Name != NULL)
    {
        ExFreePoolWithTag(BootModule->Name, HEAP_BOOT_TAG);
        BootModule->Name = NULL;
    }

    if (BootModule->BaseAddress != NULL)
    {
        ASSERT(BootModule->Length != 0);

        ExFreePoolWithTag(BootModule->BaseAddress, HEAP_BOOT_TAG);
        BootModule->BaseAddress = NULL;
    }

    BootModule->Length = 0;
}

static
void
_BootModulesUninitialize(
    _Pre_valid_ _Post_ptr_invalid_
            PBOOT_MODULE_INFORMATION            BootModules,
    IN      DWORD                               NumberOfModules
    )
{
    ASSERT(BootModules != NULL);

    for (DWORD i = 0; i < NumberOfModules; ++i)
    {
        _BootModuleUninitialize(&BootModules[i]);
    }

    ExFreePoolWithTag(BootModules, HEAP_BOOT_TAG);
}
