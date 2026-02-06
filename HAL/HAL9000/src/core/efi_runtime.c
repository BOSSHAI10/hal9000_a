#include "HAL9000.h"
#include "efi_runtime.h"

#include "common_lib.h"
#include "crc32.h"
#include "lock_common.h"

typedef struct _EFI_RUNTIME_DATA
{
    LOCK EfiRuntimeLock;

    EFI_RUNTIME_SERVICES *EfiRuntimeServices;
} EFI_RUNTIME_DATA, PEFI_RUNTIME_DATA;

static EFI_RUNTIME_DATA m_efiRuntimeData;

STATUS 
EfiRuntimePreinit(
    EFI_RUNTIME_SERVICES *EfiRuntimeServices
    )
{
    memzero(&m_efiRuntimeData, sizeof(EFI_RUNTIME_DATA));
    m_efiRuntimeData.EfiRuntimeServices = EfiRuntimeServices;

    return STATUS_SUCCESS;
}

STATUS
EfiRuntimeInit(
    void
    )
{
    UINT32 Crc32 = m_efiRuntimeData.EfiRuntimeServices->Hdr.CRC32;
    m_efiRuntimeData.EfiRuntimeServices->Hdr.CRC32 = 0;
    UINT32 Crc32Expected = ComputeCrc32((const BYTE *) m_efiRuntimeData.EfiRuntimeServices,
                                        m_efiRuntimeData.EfiRuntimeServices->Hdr.HeaderSize);

    if (Crc32Expected != Crc32)
    {
        memzero(&m_efiRuntimeData, sizeof(EFI_RUNTIME_DATA));
        return STATUS_INVALID_PARAMETER1; 
    }

    LockInit(&(m_efiRuntimeData.EfiRuntimeLock));
    LockSetName(&(m_efiRuntimeData.EfiRuntimeLock), "EfiRuntime");
    m_efiRuntimeData.EfiRuntimeServices->Hdr.CRC32 = Crc32;

    return STATUS_SUCCESS;
}

EFI_STATUS
EFIAPI
EfiRuntimeGetTime(
    EFI_TIME                    *Time,
    EFI_TIME_CAPABILITIES       *Capabilities
    )
{
    EFI_STATUS status;
    INTR_STATE intrState;

    LockAcquire(&m_efiRuntimeData.EfiRuntimeLock, &intrState);
    status = m_efiRuntimeData.EfiRuntimeServices->GetTime(Time, Capabilities); 
    LockRelease(&m_efiRuntimeData.EfiRuntimeLock, intrState);

    return status;
}

EFI_STATUS
EFIAPI
EfiRuntimeSetTime(
    EFI_TIME                     *Time
    )
{
    EFI_STATUS status;
    INTR_STATE intrState;

    LockAcquire(&m_efiRuntimeData.EfiRuntimeLock, &intrState);
    status = m_efiRuntimeData.EfiRuntimeServices->SetTime(Time);
    LockRelease(&m_efiRuntimeData.EfiRuntimeLock, intrState);

    return status;
}

EFI_STATUS
EFIAPI
EfiRuntimeGetWakeupTime(
    BOOLEAN                     *Enabled,
    BOOLEAN                     *Pending,
    EFI_TIME                    *Time
    )
{
    EFI_STATUS status;
    INTR_STATE intrState;

    LockAcquire(&m_efiRuntimeData.EfiRuntimeLock, &intrState);
    status = m_efiRuntimeData.EfiRuntimeServices->GetWakeupTime(Enabled, Pending, Time);
    LockRelease(&m_efiRuntimeData.EfiRuntimeLock, intrState);

    return status;
}

EFI_STATUS
EFIAPI
EfiRuntimeSetWakeupTime(
    BOOLEAN                      Enable,
    EFI_TIME                     *Time 
    )
{
    EFI_STATUS status;
    INTR_STATE intrState;

    LockAcquire(&m_efiRuntimeData.EfiRuntimeLock, &intrState);
    status = m_efiRuntimeData.EfiRuntimeServices->SetWakeupTime(Enable, Time);
    LockRelease(&m_efiRuntimeData.EfiRuntimeLock, intrState);

    return status;
}

EFI_STATUS
EFIAPI
EfiRuntimeGetVariable(
    CHAR16                      *VariableName,
    EFI_GUID                    *VendorGuid,
    UINT32                      *Attributes,
    UINTN                       *DataSize,
    VOID                        *Data
    )
{
    EFI_STATUS status;
    INTR_STATE intrState;

    LockAcquire(&m_efiRuntimeData.EfiRuntimeLock, &intrState);
    status = m_efiRuntimeData.EfiRuntimeServices->GetVariable(VariableName,
                                                              VendorGuid,
                                                              Attributes,
                                                              DataSize,
                                                              Data);
    LockRelease(&m_efiRuntimeData.EfiRuntimeLock, intrState);

    return status;
}


EFI_STATUS
EFIAPI
EfiRuntimeGetNextVariableName(
    UINTN                    *VariableNameSize,
    CHAR16                   *VariableName,
    EFI_GUID                 *VendorGuid
    )
{
    EFI_STATUS status;
    INTR_STATE intrState;

    LockAcquire(&m_efiRuntimeData.EfiRuntimeLock, &intrState);
    status = m_efiRuntimeData.EfiRuntimeServices->GetNextVariableName(VariableNameSize,
                                                                      VariableName,
                                                                      VendorGuid);
    LockRelease(&m_efiRuntimeData.EfiRuntimeLock, intrState);

    return status;
}

EFI_STATUS
EFIAPI
EfiRuntimeSetVariable(
    CHAR16                       *VariableName,
    EFI_GUID                     *VendorGuid,
    UINT32                       Attributes,
    UINTN                        DataSize,
    VOID                         *Data
    )
{
    EFI_STATUS status;
    INTR_STATE intrState;

    LockAcquire(&m_efiRuntimeData.EfiRuntimeLock, &intrState);
    status = m_efiRuntimeData.EfiRuntimeServices->SetVariable(VariableName,
                                                              VendorGuid,
                                                              Attributes,
                                                              DataSize,
                                                              Data);
    LockRelease(&m_efiRuntimeData.EfiRuntimeLock, intrState);

    return status;
}

EFI_STATUS
EFIAPI
EfiRuntimeGetNextHighMonotonicCount(
    UINT32                  *HighCount
    )
{
    EFI_STATUS status;
    INTR_STATE intrState;

    LockAcquire(&m_efiRuntimeData.EfiRuntimeLock, &intrState);
    status = m_efiRuntimeData.EfiRuntimeServices->GetNextHighMonotonicCount(HighCount);
    LockRelease(&m_efiRuntimeData.EfiRuntimeLock, intrState);

    return status;
}

VOID
EFIAPI
EfiRuntimeResetSystem(
    EFI_RESET_TYPE           ResetType,
    EFI_STATUS               ResetStatus,
    UINTN                    DataSize,
    VOID                     *ResetData 
    )
{
    INTR_STATE intrState;
    LockAcquire(&m_efiRuntimeData.EfiRuntimeLock, &intrState);
    m_efiRuntimeData.EfiRuntimeServices->ResetSystem(ResetType,
                                                              ResetStatus,
                                                              DataSize,
                                                              ResetData);
    NOT_REACHED // I suppose

    LockRelease(&m_efiRuntimeData.EfiRuntimeLock, intrState);
}

EFI_STATUS
EFIAPI
EfiRuntimeUpdateCapsule(
    EFI_CAPSULE_HEADER     **CapsuleHeaderArray,
    UINTN                  CapsuleCount,
    EFI_PHYSICAL_ADDRESS   ScatterGatherList
    )
{
    EFI_STATUS status;
    INTR_STATE intrState;

    LockAcquire(&m_efiRuntimeData.EfiRuntimeLock, &intrState);
    status = m_efiRuntimeData.EfiRuntimeServices->UpdateCapsule(CapsuleHeaderArray,
                                                                CapsuleCount,
                                                                ScatterGatherList);
    LockRelease(&m_efiRuntimeData.EfiRuntimeLock, intrState);

    return status;
}

EFI_STATUS
EFIAPI
EfiRuntimeQueryCapsuleCapabilities(
    EFI_CAPSULE_HEADER     **CapsuleHeaderArray,
    UINTN                  CapsuleCount,
    UINT64                 *MaximumCapsuleSize,
    EFI_RESET_TYPE         *ResetType
    )
{
    EFI_STATUS status;
    INTR_STATE intrState;

    LockAcquire(&m_efiRuntimeData.EfiRuntimeLock, &intrState);
    status = m_efiRuntimeData.EfiRuntimeServices->QueryCapsuleCapabilities(CapsuleHeaderArray,
                                                                           CapsuleCount,
                                                                           MaximumCapsuleSize,
                                                                           ResetType);
    LockRelease(&m_efiRuntimeData.EfiRuntimeLock, intrState);

    return status;
}

EFI_STATUS
EFIAPI
EfiRuntimeQueryVariableInfo(
    UINT32            Attributes,
    UINT64            *MaximumVariableStorageSize,
    UINT64            *RemainingVariableStorageSize,
    UINT64            *MaximumVariableSize
    )
{
    EFI_STATUS status;
    INTR_STATE intrState;

    LockAcquire(&m_efiRuntimeData.EfiRuntimeLock, &intrState);
    status = m_efiRuntimeData.EfiRuntimeServices->QueryVariableInfo(Attributes,
                                                                    MaximumVariableStorageSize,
                                                                    RemainingVariableStorageSize,
                                                                    MaximumVariableSize);
    LockRelease(&m_efiRuntimeData.EfiRuntimeLock, intrState);

    return status;
}
