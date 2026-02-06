#pragma once

C_HEADER_START


//******************************************************************************
// Function:     ComputeCrc32
// Description:  Compute CRC32 with the polynomial used by UEFI and the
//               HAL's Loader
// Returns:      DWORD
// Parameter:    IN BYTE *Data - Bytes for which to compute CRC32
// Parameter:    IN DWORD Length - Length of the Data
//******************************************************************************
DWORD
ComputeCrc32(
    IN BYTE *Data,
    IN DWORD Length
    );


C_HEADER_END