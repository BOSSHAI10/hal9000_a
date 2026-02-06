#pragma once

#include "uefi.h"

// TODO: Document these functions

// Simple system to warp UEFI runtime calls and also to
// add synchronization, as the specification says that
// not all functions can be called at the same time.
// There are different situations for this.
// Also a wrapper hels because if we move to ELF, we have
// to consider that UEFI uses a different calling convention
// than ELF binaries.

STATUS 
EfiRuntimePreinit(
    EFI_RUNTIME_SERVICES *EfiRuntimeServices
    );

STATUS 
EfiRuntimeInit(
    void
    );

EFI_STATUS
EFIAPI
EfiRuntimeGetTime(
    EFI_TIME                    *Time,
    EFI_TIME_CAPABILITIES       *Capabilities
    );

EFI_STATUS
EFIAPI
EfiRuntimeSetTime(
    EFI_TIME                     *Time
    );

EFI_STATUS
EFIAPI
EfiRuntimeGetWakeupTime(
    BOOLEAN                     *Enabled,
    BOOLEAN                     *Pending,
    EFI_TIME                    *Time
    );

EFI_STATUS
EFIAPI
EfiRuntimeSetWakeupTime(
    BOOLEAN                      Enable,
    EFI_TIME                     *Time 
    );

EFI_STATUS
EFIAPI
EfiRuntimeGetVariable(
    CHAR16                      *VariableName,
    EFI_GUID                    *VendorGuid,
    UINT32                      *Attributes,
    UINTN                       *DataSize,
    VOID                        *Data
    );


EFI_STATUS
EFIAPI
EfiRuntimeGetNextVariableName(
    UINTN                    *VariableNameSize,
    CHAR16                   *VariableName,
    EFI_GUID                 *VendorGuid
    );

EFI_STATUS
EFIAPI
EfiRuntimeSetVariable(
    CHAR16                       *VariableName,
    EFI_GUID                     *VendorGuid,
    UINT32                       Attributes,
    UINTN                        DataSize,
    VOID                         *Data
    );

EFI_STATUS
EFIAPI
EfiRuntimeGetNextHighMonotonicCount(
    UINT32                  *HighCount
    );

VOID
EFIAPI
EfiRuntimeResetSystem(
    EFI_RESET_TYPE           ResetType,
    EFI_STATUS               ResetStatus,
    UINTN                    DataSize,
    VOID                     *ResetData 
    );

EFI_STATUS
EFIAPI
EfiRuntimeUpdateCapsule(
    EFI_CAPSULE_HEADER     **CapsuleHeaderArray,
    UINTN                  CapsuleCount,
    EFI_PHYSICAL_ADDRESS   ScatterGatherList
    );

EFI_STATUS
EFIAPI
EfiRuntimeQueryCapsuleCapabilities(
    EFI_CAPSULE_HEADER     **CapsuleHeaderArray,
    UINTN                  CapsuleCount,
    UINT64                 *MaximumCapsuleSize,
    EFI_RESET_TYPE         *ResetType
    );

EFI_STATUS
EFIAPI
EfiRuntimeQueryVariableInfo(
    UINT32            Attributes,
    UINT64            *MaximumVariableStorageSize,
    UINT64            *RemainingVariableStorageSize,
    UINT64            *MaximumVariableSize
    );
