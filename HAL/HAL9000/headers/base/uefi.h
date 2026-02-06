#pragma once

#include "cal_annotate.h"
#include "data_type.h"

// I could not include all of UEFI because of macro and type redefinitions
// Parts of the EDK2 UEFI Headers

/**
  Returns a 16-bit signature built from 2 ASCII characters.

  This macro returns a 16-bit value built from the two ASCII characters specified
  by A and B.

  @param  A    The first ASCII character.
  @param  B    The second ASCII character.

  @return A 16-bit value built from the two ASCII characters specified by A and B.

**/
#define SIGNATURE_16(A, B)  ((A) | (B << 8))

/**
  Returns a 32-bit signature built from 4 ASCII characters.

  This macro returns a 32-bit value built from the four ASCII characters specified
  by A, B, C, and D.

  @param  A    The first ASCII character.
  @param  B    The second ASCII character.
  @param  C    The third ASCII character.
  @param  D    The fourth ASCII character.

  @return A 32-bit value built from the two ASCII characters specified by A, B,
          C and D.

**/
#define SIGNATURE_32(A, B, C, D)  (SIGNATURE_16 (A, B) | (SIGNATURE_16 (C, D) << 16))

/**
  Returns a 64-bit signature built from 8 ASCII characters.

  This macro returns a 64-bit value built from the eight ASCII characters specified
  by A, B, C, D, E, F, G,and H.

  @param  A    The first ASCII character.
  @param  B    The second ASCII character.
  @param  C    The third ASCII character.
  @param  D    The fourth ASCII character.
  @param  E    The fifth ASCII character.
  @param  F    The sixth ASCII character.
  @param  G    The seventh ASCII character.
  @param  H    The eighth ASCII character.

  @return A 64-bit value built from the two ASCII characters specified by A, B,
          C, D, E, F, G and H.

**/
#define SIGNATURE_64(A, B, C, D, E, F, G, H) \
    (SIGNATURE_32 (A, B, C, D) | ((UINT64) (SIGNATURE_32 (E, F, G, H)) << 32))



#define EFIAPI MS_ABI CDECL

typedef VOID *EFI_HANDLE;

typedef UINTN RETURN_STATUS;

///
/// A value of native width with the highest bit set.
///
#define MAX_BIT  0x8000000000000000ULL

/**
  Produces a RETURN_STATUS code with the highest bit set.

  @param  StatusCode    The status code value to convert into a warning code.
                        StatusCode must be in the range 0x00000000..0x7FFFFFFF.

  @return The value specified by StatusCode with the highest bit set.

**/
#define ENCODE_ERROR(StatusCode)  ((RETURN_STATUS)(MAX_BIT | (StatusCode)))

/**
  Produces a RETURN_STATUS code with the highest bit clear.

  @param  StatusCode    The status code value to convert into a warning code.
                        StatusCode must be in the range 0x00000000..0x7FFFFFFF.

  @return The value specified by StatusCode with the highest bit clear.

**/
#define ENCODE_WARNING(StatusCode)  ((RETURN_STATUS)(StatusCode))

/**
  Returns TRUE if a specified RETURN_STATUS code is an error code.

  This function returns TRUE if StatusCode has the high bit set.  Otherwise, FALSE is returned.

  @param  StatusCode    The status code value to evaluate.

  @retval TRUE          The high bit of StatusCode is set.
  @retval FALSE         The high bit of StatusCode is clear.

**/
#define RETURN_ERROR(StatusCode)  (((INTN)(RETURN_STATUS)(StatusCode)) < 0)

///
/// The operation completed successfully.
///
#define RETURN_SUCCESS  (RETURN_STATUS)(0)

///
/// The image failed to load.
///
#define RETURN_LOAD_ERROR  ENCODE_ERROR (1)

///
/// The parameter was incorrect.
///
#define RETURN_INVALID_PARAMETER  ENCODE_ERROR (2)

///
/// The operation is not supported.
///
#define RETURN_UNSUPPORTED  ENCODE_ERROR (3)

///
/// The buffer was not the proper size for the request.
///
#define RETURN_BAD_BUFFER_SIZE  ENCODE_ERROR (4)

///
/// The buffer was not large enough to hold the requested data.
/// The required buffer size is returned in the appropriate
/// parameter when this error occurs.
///
#define RETURN_BUFFER_TOO_SMALL  ENCODE_ERROR (5)

///
/// There is no data pending upon return.
///
#define RETURN_NOT_READY  ENCODE_ERROR (6)

///
/// The physical device reported an error while attempting the
/// operation.
///
#define RETURN_DEVICE_ERROR  ENCODE_ERROR (7)

///
/// The device can not be written to.
///
#define RETURN_WRITE_PROTECTED  ENCODE_ERROR (8)

///
/// The resource has run out.
///
#define RETURN_OUT_OF_RESOURCES  ENCODE_ERROR (9)

///
/// An inconsistency was detected on the file system causing the
/// operation to fail.
///
#define RETURN_VOLUME_CORRUPTED  ENCODE_ERROR (10)

///
/// There is no more space on the file system.
///
#define RETURN_VOLUME_FULL  ENCODE_ERROR (11)

///
/// The device does not contain any medium to perform the
/// operation.
///
#define RETURN_NO_MEDIA  ENCODE_ERROR (12)

///
/// The medium in the device has changed since the last
/// access.
///
#define RETURN_MEDIA_CHANGED  ENCODE_ERROR (13)

///
/// The item was not found.
///
#define RETURN_NOT_FOUND  ENCODE_ERROR (14)

///
/// Access was denied.
///
#define RETURN_ACCESS_DENIED  ENCODE_ERROR (15)

///
/// The server was not found or did not respond to the request.
///
#define RETURN_NO_RESPONSE  ENCODE_ERROR (16)

///
/// A mapping to the device does not exist.
///
#define RETURN_NO_MAPPING  ENCODE_ERROR (17)

///
/// A timeout time expired.
///
#define RETURN_TIMEOUT  ENCODE_ERROR (18)

///
/// The protocol has not been started.
///
#define RETURN_NOT_STARTED  ENCODE_ERROR (19)

///
/// The protocol has already been started.
///
#define RETURN_ALREADY_STARTED  ENCODE_ERROR (20)

///
/// The operation was aborted.
///
#define RETURN_ABORTED  ENCODE_ERROR (21)

///
/// An ICMP error occurred during the network operation.
///
#define RETURN_ICMP_ERROR  ENCODE_ERROR (22)

///
/// A TFTP error occurred during the network operation.
///
#define RETURN_TFTP_ERROR  ENCODE_ERROR (23)

///
/// A protocol error occurred during the network operation.
///
#define RETURN_PROTOCOL_ERROR  ENCODE_ERROR (24)

///
/// A function encountered an internal version that was
/// incompatible with a version requested by the caller.
///
#define RETURN_INCOMPATIBLE_VERSION  ENCODE_ERROR (25)

///
/// The function was not performed due to a security violation.
///
#define RETURN_SECURITY_VIOLATION  ENCODE_ERROR (26)

///
/// A CRC error was detected.
///
#define RETURN_CRC_ERROR  ENCODE_ERROR (27)

///
/// The beginning or end of media was reached.
///
#define RETURN_END_OF_MEDIA  ENCODE_ERROR (28)

///
/// The end of the file was reached.
///
#define RETURN_END_OF_FILE  ENCODE_ERROR (31)

///
/// The language specified was invalid.
///
#define RETURN_INVALID_LANGUAGE  ENCODE_ERROR (32)

///
/// The security status of the data is unknown or compromised
/// and the data must be updated or replaced to restore a valid
/// security status.
///
#define RETURN_COMPROMISED_DATA  ENCODE_ERROR (33)

///
/// There is an address conflict address allocation.
///
#define RETURN_IP_ADDRESS_CONFLICT  ENCODE_ERROR (34)

///
/// A HTTP error occurred during the network operation.
///
#define RETURN_HTTP_ERROR  ENCODE_ERROR (35)

///
/// The string contained one or more characters that
/// the device could not render and were skipped.
///
#define RETURN_WARN_UNKNOWN_GLYPH  ENCODE_WARNING (1)

///
/// The handle was closed, but the file was not deleted.
///
#define RETURN_WARN_DELETE_FAILURE  ENCODE_WARNING (2)

///
/// The handle was closed, but the data to the file was not
/// flushed properly.
///
#define RETURN_WARN_WRITE_FAILURE  ENCODE_WARNING (3)

///
/// The resulting buffer was too small, and the data was
/// truncated to the buffer size.
///
#define RETURN_WARN_BUFFER_TOO_SMALL  ENCODE_WARNING (4)

///
/// The data has not been updated within the timeframe set by
/// local policy for this type of data.
///
#define RETURN_WARN_STALE_DATA  ENCODE_WARNING (5)

///
/// The resulting buffer contains UEFI-compliant file system.
///
#define RETURN_WARN_FILE_SYSTEM  ENCODE_WARNING (6)

///
/// The operation will be processed across a system reset.
///
#define RETURN_WARN_RESET_REQUIRED  ENCODE_WARNING (7)

typedef RETURN_STATUS EFI_STATUS;

///
/// Enumeration of EFI_STATUS.
///@{
#define EFI_SUCCESS               RETURN_SUCCESS
#define EFI_LOAD_ERROR            RETURN_LOAD_ERROR
#define EFI_INVALID_PARAMETER     RETURN_INVALID_PARAMETER
#define EFI_UNSUPPORTED           RETURN_UNSUPPORTED
#define EFI_BAD_BUFFER_SIZE       RETURN_BAD_BUFFER_SIZE
#define EFI_BUFFER_TOO_SMALL      RETURN_BUFFER_TOO_SMALL
#define EFI_NOT_READY             RETURN_NOT_READY
#define EFI_DEVICE_ERROR          RETURN_DEVICE_ERROR
#define EFI_WRITE_PROTECTED       RETURN_WRITE_PROTECTED
#define EFI_OUT_OF_RESOURCES      RETURN_OUT_OF_RESOURCES
#define EFI_VOLUME_CORRUPTED      RETURN_VOLUME_CORRUPTED
#define EFI_VOLUME_FULL           RETURN_VOLUME_FULL
#define EFI_NO_MEDIA              RETURN_NO_MEDIA
#define EFI_MEDIA_CHANGED         RETURN_MEDIA_CHANGED
#define EFI_NOT_FOUND             RETURN_NOT_FOUND
#define EFI_ACCESS_DENIED         RETURN_ACCESS_DENIED
#define EFI_NO_RESPONSE           RETURN_NO_RESPONSE
#define EFI_NO_MAPPING            RETURN_NO_MAPPING
#define EFI_TIMEOUT               RETURN_TIMEOUT
#define EFI_NOT_STARTED           RETURN_NOT_STARTED
#define EFI_ALREADY_STARTED       RETURN_ALREADY_STARTED
#define EFI_ABORTED               RETURN_ABORTED
#define EFI_ICMP_ERROR            RETURN_ICMP_ERROR
#define EFI_TFTP_ERROR            RETURN_TFTP_ERROR
#define EFI_PROTOCOL_ERROR        RETURN_PROTOCOL_ERROR
#define EFI_INCOMPATIBLE_VERSION  RETURN_INCOMPATIBLE_VERSION
#define EFI_SECURITY_VIOLATION    RETURN_SECURITY_VIOLATION
#define EFI_CRC_ERROR             RETURN_CRC_ERROR
#define EFI_END_OF_MEDIA          RETURN_END_OF_MEDIA
#define EFI_END_OF_FILE           RETURN_END_OF_FILE
#define EFI_INVALID_LANGUAGE      RETURN_INVALID_LANGUAGE
#define EFI_COMPROMISED_DATA      RETURN_COMPROMISED_DATA
#define EFI_IP_ADDRESS_CONFLICT   RETURN_IP_ADDRESS_CONFLICT
#define EFI_HTTP_ERROR            RETURN_HTTP_ERROR

#define EFI_WARN_UNKNOWN_GLYPH     RETURN_WARN_UNKNOWN_GLYPH
#define EFI_WARN_DELETE_FAILURE    RETURN_WARN_DELETE_FAILURE
#define EFI_WARN_WRITE_FAILURE     RETURN_WARN_WRITE_FAILURE
#define EFI_WARN_BUFFER_TOO_SMALL  RETURN_WARN_BUFFER_TOO_SMALL
#define EFI_WARN_STALE_DATA        RETURN_WARN_STALE_DATA
#define EFI_WARN_FILE_SYSTEM       RETURN_WARN_FILE_SYSTEM
#define EFI_WARN_RESET_REQUIRED    RETURN_WARN_RESET_REQUIRED
///@}

///
/// 128 bit buffer containing a unique identifier value.
/// Unless otherwise specified, aligned on a 64 bit boundary.
///
typedef struct {
  UINT32    Data1;
  UINT16    Data2;
  UINT16    Data3;
  UINT8     Data4[8];
} GUID;

///
/// 128-bit buffer containing a unique identifier value.
///
typedef GUID EFI_GUID;

///
/// 64-bit physical memory address.
///
typedef UINT64 EFI_PHYSICAL_ADDRESS;

///
/// 64-bit virtual memory address.
///
typedef UINT64 EFI_VIRTUAL_ADDRESS;

///
/// Enumeration of EFI memory allocation types.
///
typedef enum {
  ///
  /// Allocate any available range of pages that satisfies the request.
  ///
  AllocateAnyPages,
  ///
  /// Allocate any available range of pages whose uppermost address is less than
  /// or equal to a specified maximum address.
  ///
  AllocateMaxAddress,
  ///
  /// Allocate pages at a specified address.
  ///
  AllocateAddress,
  ///
  /// Maximum enumeration value that may be used for bounds checking.
  ///
  MaxAllocateType
} EFI_ALLOCATE_TYPE;

///
/// Enumeration of memory types introduced in UEFI.
///
typedef enum {
  ///
  /// Not used.
  ///
  EfiReservedMemoryType,
  ///
  /// The code portions of a loaded application.
  /// (Note that UEFI OS loaders are UEFI applications.)
  ///
  EfiLoaderCode,
  ///
  /// The data portions of a loaded application and the default data allocation
  /// type used by an application to allocate pool memory.
  ///
  EfiLoaderData,
  ///
  /// The code portions of a loaded Boot Services Driver.
  ///
  EfiBootServicesCode,
  ///
  /// The data portions of a loaded Boot Serves Driver, and the default data
  /// allocation type used by a Boot Services Driver to allocate pool memory.
  ///
  EfiBootServicesData,
  ///
  /// The code portions of a loaded Runtime Services Driver.
  ///
  EfiRuntimeServicesCode,
  ///
  /// The data portions of a loaded Runtime Services Driver and the default
  /// data allocation type used by a Runtime Services Driver to allocate pool memory.
  ///
  EfiRuntimeServicesData,
  ///
  /// Free (unallocated) memory.
  ///
  EfiConventionalMemory,
  ///
  /// Memory in which errors have been detected.
  ///
  EfiUnusableMemory,
  ///
  /// Memory that holds the ACPI tables.
  ///
  EfiACPIReclaimMemory,
  ///
  /// Address space reserved for use by the firmware.
  ///
  EfiACPIMemoryNVS,
  ///
  /// Used by system firmware to request that a memory-mapped IO region
  /// be mapped by the OS to a virtual address so it can be accessed by EFI runtime services.
  ///
  EfiMemoryMappedIO,
  ///
  /// System memory-mapped IO region that is used to translate memory
  /// cycles to IO cycles by the processor.
  ///
  EfiMemoryMappedIOPortSpace,
  ///
  /// Address space reserved by the firmware for code that is part of the processor.
  ///
  EfiPalCode,
  ///
  /// A memory region that operates as EfiConventionalMemory,
  /// however it happens to also support byte-addressable non-volatility.
  ///
  EfiPersistentMemory,
  ///
  /// A memory region that describes system memory that has not been accepted
  /// by a corresponding call to the underlying isolation architecture.
  ///
  EfiUnacceptedMemoryType,
  EfiMaxMemoryType,
  //
  // +---------------------------------------------------+
  // | 0..(EfiMaxMemoryType - 1)    - Normal memory type |
  // +---------------------------------------------------+
  // | EfiMaxMemoryType..0x6FFFFFFF - Invalid            |
  // +---------------------------------------------------+
  // | 0x70000000..0x7FFFFFFF       - OEM reserved       |
  // +---------------------------------------------------+
  // | 0x80000000..0xFFFFFFFF       - OS reserved        |
  // +---------------------------------------------------+
  //
  MEMORY_TYPE_OEM_RESERVED_MIN = 0x70000000,
  MEMORY_TYPE_OEM_RESERVED_MAX = 0x7FFFFFFF,
  MEMORY_TYPE_OS_RESERVED_MIN  = 0x80000000,
  MEMORY_TYPE_OS_RESERVED_MAX  = 0xFFFFFFFF
} EFI_MEMORY_TYPE;

///
/// Enumeration of reset types.
///
typedef enum {
  ///
  /// Used to induce a system-wide reset. This sets all circuitry within the
  /// system to its initial state.  This type of reset is asynchronous to system
  /// operation and operates withgout regard to cycle boundaries.  EfiColdReset
  /// is tantamount to a system power cycle.
  ///
  EfiResetCold,
  ///
  /// Used to induce a system-wide initialization. The processors are set to their
  /// initial state, and pending cycles are not corrupted.  If the system does
  /// not support this reset type, then an EfiResetCold must be performed.
  ///
  EfiResetWarm,
  ///
  /// Used to induce an entry into a power state equivalent to the ACPI G2/S5 or G3
  /// state.  If the system does not support this reset type, then when the system
  /// is rebooted, it should exhibit the EfiResetCold attributes.
  ///
  EfiResetShutdown,
  ///
  /// Used to induce a system-wide reset. The exact type of the reset is defined by
  /// the EFI_GUID that follows the Null-terminated Unicode string passed into
  /// ResetData. If the platform does not recognize the EFI_GUID in ResetData the
  /// platform must pick a supported reset type to perform. The platform may
  /// optionally log the parameters from any non-normal reset that occurs.
  ///
  EfiResetPlatformSpecific
} EFI_RESET_TYPE;

//
// Bit definitions for EFI_TIME.Daylight
//
#define EFI_TIME_ADJUST_DAYLIGHT  0x01
#define EFI_TIME_IN_DAYLIGHT      0x02

///
/// Value definition for EFI_TIME.TimeZone.
///
#define EFI_UNSPECIFIED_TIMEZONE  0x07FF

//
// Memory cacheability attributes
//
#define EFI_MEMORY_UC   0x0000000000000001ULL
#define EFI_MEMORY_WC   0x0000000000000002ULL
#define EFI_MEMORY_WT   0x0000000000000004ULL
#define EFI_MEMORY_WB   0x0000000000000008ULL
#define EFI_MEMORY_UCE  0x0000000000000010ULL
//
// Physical memory protection attributes
//
// Note: UEFI spec 2.5 and following: use EFI_MEMORY_RO as write-protected physical memory
// protection attribute. Also, EFI_MEMORY_WP means cacheability attribute.
//
#define EFI_MEMORY_WP  0x0000000000001000ULL
#define EFI_MEMORY_RP  0x0000000000002000ULL
#define EFI_MEMORY_XP  0x0000000000004000ULL
#define EFI_MEMORY_RO  0x0000000000020000ULL
//
// Physical memory persistence attribute.
// The memory region supports byte-addressable non-volatility.
//
#define EFI_MEMORY_NV  0x0000000000008000ULL
//
// The memory region provides higher reliability relative to other memory in the system.
// If all memory has the same reliability, then this bit is not used.
//
#define EFI_MEMORY_MORE_RELIABLE  0x0000000000010000ULL

//
// Note: UEFI spec 2.8 and following:
//
// Specific-purpose memory (SPM). The memory is earmarked for
// specific purposes such as for specific device drivers or applications.
// The SPM attribute serves as a hint to the OS to avoid allocating this
// memory for core OS data or code that can not be relocated.
//
#define EFI_MEMORY_SP  0x0000000000040000ULL
//
// If this flag is set, the memory region is capable of being
// protected with the CPU's memory cryptographic
// capabilities. If this flag is clear, the memory region is not
// capable of being protected with the CPU's memory
// cryptographic capabilities or the CPU does not support CPU
// memory cryptographic capabilities.
//
#define EFI_MEMORY_CPU_CRYPTO  0x0000000000080000ULL

//
// Runtime memory attribute
//
#define EFI_MEMORY_RUNTIME  0x8000000000000000ULL

//
// If this flag is set, the memory region is
// described with additional ISA-specific memory attributes
// as specified in EFI_MEMORY_ISA_MASK.
//
#define EFI_MEMORY_ISA_VALID  0x4000000000000000ULL

//
// Defines the bits reserved for describing optional ISA-specific cacheability
// attributes that are not covered by the standard UEFI Memory Attributes cacheability
// bits (EFI_MEMORY_UC, EFI_MEMORY_WC, EFI_MEMORY_WT, EFI_MEMORY_WB and EFI_MEMORY_UCE).
// See Calling Conventions for further ISA-specific enumeration of these bits.
//
#define EFI_MEMORY_ISA_MASK  0x0FFFF00000000000ULL

//
// Attributes bitmasks, grouped by type
//
#define EFI_CACHE_ATTRIBUTE_MASK   (EFI_MEMORY_UC | EFI_MEMORY_WC | EFI_MEMORY_WT | EFI_MEMORY_WB | EFI_MEMORY_UCE | EFI_MEMORY_WP)
#define EFI_MEMORY_ACCESS_MASK     (EFI_MEMORY_RP | EFI_MEMORY_XP | EFI_MEMORY_RO)
#define EFI_MEMORY_ATTRIBUTE_MASK  (EFI_MEMORY_ACCESS_MASK | EFI_MEMORY_SP | EFI_MEMORY_CPU_CRYPTO)

///
/// Memory descriptor version number.
///
#define EFI_MEMORY_DESCRIPTOR_VERSION  1

///
/// Definition of an EFI memory descriptor.
///
typedef struct {
  ///
  /// Type of the memory region.
  /// Type EFI_MEMORY_TYPE is defined in the
  /// AllocatePages() function description.
  ///
  UINT32                  Type;
  ///
  /// Physical address of the first byte in the memory region. PhysicalStart must be
  /// aligned on a 4 KiB boundary, and must not be above 0xfffffffffffff000. Type
  /// EFI_PHYSICAL_ADDRESS is defined in the AllocatePages() function description
  ///
  EFI_PHYSICAL_ADDRESS    PhysicalStart;
  ///
  /// Virtual address of the first byte in the memory region.
  /// VirtualStart must be aligned on a 4 KiB boundary,
  /// and must not be above 0xfffffffffffff000.
  ///
  EFI_VIRTUAL_ADDRESS     VirtualStart;
  ///
  /// NumberOfPagesNumber of 4 KiB pages in the memory region.
  /// NumberOfPages must not be 0, and must not be any value
  /// that would represent a memory page with a start address,
  /// either physical or virtual, above 0xfffffffffffff000.
  ///
  UINT64                  NumberOfPages;
  ///
  /// Attributes of the memory region that describe the bit mask of capabilities
  /// for that memory region, and not necessarily the current settings for that
  /// memory region.
  ///
  UINT64                  Attribute;
} EFI_MEMORY_DESCRIPTOR;

///
/// EFI Capsule Block Descriptor
///
typedef struct {
  ///
  /// Length in bytes of the data pointed to by DataBlock/ContinuationPointer.
  ///
  UINT64    Length;
  union {
    ///
    /// Physical address of the data block. This member of the union is
    /// used if Length is not equal to zero.
    ///
    EFI_PHYSICAL_ADDRESS    DataBlock;
    ///
    /// Physical address of another block of
    /// EFI_CAPSULE_BLOCK_DESCRIPTOR structures. This
    /// member of the union is used if Length is equal to zero. If
    /// ContinuationPointer is zero this entry represents the end of the list.
    ///
    EFI_PHYSICAL_ADDRESS    ContinuationPointer;
  } Union;
} EFI_CAPSULE_BLOCK_DESCRIPTOR;

///
/// EFI Capsule Header.
///
typedef struct {
  ///
  /// A GUID that defines the contents of a capsule.
  ///
  EFI_GUID    CapsuleGuid;
  ///
  /// The size of the capsule header. This may be larger than the size of
  /// the EFI_CAPSULE_HEADER since CapsuleGuid may imply
  /// extended header entries
  ///
  UINT32      HeaderSize;
  ///
  /// Bit-mapped list describing the capsule attributes. The Flag values
  /// of 0x0000 - 0xFFFF are defined by CapsuleGuid. Flag values
  /// of 0x10000 - 0xFFFFFFFF are defined by this specification
  ///
  UINT32      Flags;
  ///
  /// Size in bytes of the capsule (including capsule header).
  ///
  UINT32      CapsuleImageSize;
} EFI_CAPSULE_HEADER;

///
/// The EFI System Table entry must point to an array of capsules
/// that contain the same CapsuleGuid value. The array must be
/// prefixed by a UINT32 that represents the size of the array of capsules.
///
typedef struct {
  ///
  /// the size of the array of capsules.
  ///
  UINT32    CapsuleArrayNumber;
  ///
  /// Point to an array of capsules that contain the same CapsuleGuid value.
  ///
  VOID      *CapsulePtr[1];
} EFI_CAPSULE_TABLE;

#define CAPSULE_FLAGS_PERSIST_ACROSS_RESET   0x00010000
#define CAPSULE_FLAGS_POPULATE_SYSTEM_TABLE  0x00020000
#define CAPSULE_FLAGS_INITIATE_RESET         0x00040000

///
/// EFI Time Abstraction:
///  Year:       1900 - 9999
///  Month:      1 - 12
///  Day:        1 - 31
///  Hour:       0 - 23
///  Minute:     0 - 59
///  Second:     0 - 59
///  Nanosecond: 0 - 999,999,999
///  TimeZone:   -1440 to 1440 or 2047
///
typedef struct {
  UINT16    Year;
  UINT8     Month;
  UINT8     Day;
  UINT8     Hour;
  UINT8     Minute;
  UINT8     Second;
  UINT8     Pad1;
  UINT32    Nanosecond;
  INT16     TimeZone;
  UINT8     Daylight;
  UINT8     Pad2;
} EFI_TIME;

///
/// This provides the capabilities of the
/// real time clock device as exposed through the EFI interfaces.
///
typedef struct {
  ///
  /// Provides the reporting resolution of the real-time clock device in
  /// counts per second. For a normal PC-AT CMOS RTC device, this
  /// value would be 1 Hz, or 1, to indicate that the device only reports
  /// the time to the resolution of 1 second.
  ///
  UINT32     Resolution;
  ///
  /// Provides the timekeeping accuracy of the real-time clock in an
  /// error rate of 1E-6 parts per million. For a clock with an accuracy
  /// of 50 parts per million, the value in this field would be
  /// 50,000,000.
  ///
  UINT32     Accuracy;
  ///
  /// A TRUE indicates that a time set operation clears the device's
  /// time below the Resolution reporting level. A FALSE
  /// indicates that the state below the Resolution level of the
  /// device is not cleared when the time is set. Normal PC-AT CMOS
  /// RTC devices set this value to FALSE.
  ///
  BOOLEAN    SetsToZero;
} EFI_TIME_CAPABILITIES;

/**
  Returns the current time and date information, and the time-keeping capabilities
  of the hardware platform.

  @param[out]  Time             A pointer to storage to receive a snapshot of the current time.
  @param[out]  Capabilities     An optional pointer to a buffer to receive the real time clock
                                device's capabilities.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval EFI_INVALID_PARAMETER Time is NULL.
  @retval EFI_DEVICE_ERROR      The time could not be retrieved due to hardware error.
  @retval EFI_UNSUPPORTED       This call is not supported by this platform at the time the call is made.
                                The platform should describe this runtime service as unsupported at runtime
                                via an EFI_RT_PROPERTIES_TABLE configuration table.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_GET_TIME)(
  EFI_TIME                    *Time,
  EFI_TIME_CAPABILITIES       *Capabilities
  );

/**
  Sets the current local time and date information.

  @param[in]  Time              A pointer to the current time.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval EFI_INVALID_PARAMETER A time field is out of range.
  @retval EFI_DEVICE_ERROR      The time could not be set due due to hardware error.
  @retval EFI_UNSUPPORTED       This call is not supported by this platform at the time the call is made.
                                The platform should describe this runtime service as unsupported at runtime
                                via an EFI_RT_PROPERTIES_TABLE configuration table.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_TIME)(
  EFI_TIME                     *Time
  );

/**
  Returns the current wakeup alarm clock setting.

  @param[out]  Enabled          Indicates if the alarm is currently enabled or disabled.
  @param[out]  Pending          Indicates if the alarm signal is pending and requires acknowledgement.
  @param[out]  Time             The current alarm setting.

  @retval EFI_SUCCESS           The alarm settings were returned.
  @retval EFI_INVALID_PARAMETER Enabled is NULL.
  @retval EFI_INVALID_PARAMETER Pending is NULL.
  @retval EFI_INVALID_PARAMETER Time is NULL.
  @retval EFI_DEVICE_ERROR      The wakeup time could not be retrieved due to a hardware error.
  @retval EFI_UNSUPPORTED       This call is not supported by this platform at the time the call is made.
                                The platform should describe this runtime service as unsupported at runtime
                                via an EFI_RT_PROPERTIES_TABLE configuration table.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_GET_WAKEUP_TIME)(
  BOOLEAN                     *Enabled,
  BOOLEAN                     *Pending,
  EFI_TIME                    *Time
  );

/**
  Sets the system wakeup alarm clock time.

  @param[in]  Enable            Enable or disable the wakeup alarm.
  @param[in]  Time              If Enable is TRUE, the time to set the wakeup alarm for.
                                If Enable is FALSE, then this parameter is optional, and may be NULL.

  @retval EFI_SUCCESS           If Enable is TRUE, then the wakeup alarm was enabled. If
                                Enable is FALSE, then the wakeup alarm was disabled.
  @retval EFI_INVALID_PARAMETER A time field is out of range.
  @retval EFI_DEVICE_ERROR      The wakeup time could not be set due to a hardware error.
  @retval EFI_UNSUPPORTED       This call is not supported by this platform at the time the call is made.
                                The platform should describe this runtime service as unsupported at runtime
                                via an EFI_RT_PROPERTIES_TABLE configuration table.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_WAKEUP_TIME)(
  BOOLEAN                      Enable,
  EFI_TIME                     *Time 
  );

/**
  Changes the runtime addressing mode of EFI firmware from physical to virtual.

  @param[in]  MemoryMapSize     The size in bytes of VirtualMap.
  @param[in]  DescriptorSize    The size in bytes of an entry in the VirtualMap.
  @param[in]  DescriptorVersion The version of the structure entries in VirtualMap.
  @param[in]  VirtualMap        An array of memory descriptors which contain new virtual
                                address mapping information for all runtime ranges.

  @retval EFI_SUCCESS           The virtual address map has been applied.
  @retval EFI_UNSUPPORTED       EFI firmware is not at runtime, or the EFI firmware is already in
                                virtual address mapped mode.
  @retval EFI_INVALID_PARAMETER DescriptorSize or DescriptorVersion is invalid.
  @retval EFI_NO_MAPPING        A virtual address was not supplied for a range in the memory
                                map that requires a mapping.
  @retval EFI_NOT_FOUND         A virtual address was supplied for an address that is not found
                                in the memory map.
  @retval EFI_UNSUPPORTED       This call is not supported by this platform at the time the call is made.
                                The platform should describe this runtime service as unsupported at runtime
                                via an EFI_RT_PROPERTIES_TABLE configuration table.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_VIRTUAL_ADDRESS_MAP)(
  UINTN                        MemoryMapSize,
  UINTN                        DescriptorSize,
  UINT32                       DescriptorVersion,
  EFI_MEMORY_DESCRIPTOR        *VirtualMap
  );

//
// ConvertPointer DebugDisposition type.
//
#define EFI_OPTIONAL_PTR  0x00000001

/**
  Determines the new virtual address that is to be used on subsequent memory accesses.

  @param[in]       DebugDisposition  Supplies type information for the pointer being converted.
  @param[in, out]  Address           A pointer to a pointer that is to be fixed to be the value needed
                                     for the new virtual address mappings being applied.

  @retval EFI_SUCCESS           The pointer pointed to by Address was modified.
  @retval EFI_NOT_FOUND         The pointer pointed to by Address was not found to be part
                                of the current memory map. This is normally fatal.
  @retval EFI_INVALID_PARAMETER Address is NULL.
  @retval EFI_INVALID_PARAMETER *Address is NULL and DebugDisposition does
                                not have the EFI_OPTIONAL_PTR bit set.
  @retval EFI_UNSUPPORTED       This call is not supported by this platform at the time the call is made.
                                The platform should describe this runtime service as unsupported at runtime
                                via an EFI_RT_PROPERTIES_TABLE configuration table.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_CONVERT_POINTER)(
  UINTN                      DebugDisposition,
  VOID                       **Address
  );

/**
  Returns the value of a variable.

  @param[in]       VariableName  A Null-terminated string that is the name of the vendor's
                                 variable.
  @param[in]       VendorGuid    A unique identifier for the vendor.
  @param[out]      Attributes    If not NULL, a pointer to the memory location to return the
                                 attributes bitmask for the variable.
  @param[in, out]  DataSize      On input, the size in bytes of the return Data buffer.
                                 On output the size of data returned in Data.
  @param[out]      Data          The buffer to return the contents of the variable. May be NULL
                                 with a zero DataSize in order to determine the size buffer needed.

  @retval EFI_SUCCESS            The function completed successfully.
  @retval EFI_NOT_FOUND          The variable was not found.
  @retval EFI_BUFFER_TOO_SMALL   The DataSize is too small for the result.
  @retval EFI_INVALID_PARAMETER  VariableName is NULL.
  @retval EFI_INVALID_PARAMETER  VendorGuid is NULL.
  @retval EFI_INVALID_PARAMETER  DataSize is NULL.
  @retval EFI_INVALID_PARAMETER  The DataSize is not too small and Data is NULL.
  @retval EFI_DEVICE_ERROR       The variable could not be retrieved due to a hardware error.
  @retval EFI_SECURITY_VIOLATION The variable could not be retrieved due to an authentication failure.
  @retval EFI_UNSUPPORTED        After ExitBootServices() has been called, this return code may be returned
                                 if no variable storage is supported. The platform should describe this
                                 runtime service as unsupported at runtime via an EFI_RT_PROPERTIES_TABLE
                                 configuration table.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_GET_VARIABLE)(
  CHAR16                      *VariableName,
  EFI_GUID                    *VendorGuid,
  UINT32                      *Attributes,
  UINTN                       *DataSize,
  VOID                        *Data
  );

/**
  Enumerates the current variable names.

  @param[in, out]  VariableNameSize The size of the VariableName buffer. The size must be large
                                    enough to fit input string supplied in VariableName buffer.
  @param[in, out]  VariableName     On input, supplies the last VariableName that was returned
                                    by GetNextVariableName(). On output, returns the Nullterminated
                                    string of the current variable.
  @param[in, out]  VendorGuid       On input, supplies the last VendorGuid that was returned by
                                    GetNextVariableName(). On output, returns the
                                    VendorGuid of the current variable.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_NOT_FOUND         The next variable was not found.
  @retval EFI_BUFFER_TOO_SMALL  The VariableNameSize is too small for the result.
                                VariableNameSize has been updated with the size needed to complete the request.
  @retval EFI_INVALID_PARAMETER VariableNameSize is NULL.
  @retval EFI_INVALID_PARAMETER VariableName is NULL.
  @retval EFI_INVALID_PARAMETER VendorGuid is NULL.
  @retval EFI_INVALID_PARAMETER The input values of VariableName and VendorGuid are not a name and
                                GUID of an existing variable.
  @retval EFI_INVALID_PARAMETER Null-terminator is not found in the first VariableNameSize bytes of
                                the input VariableName buffer.
  @retval EFI_DEVICE_ERROR      The variable could not be retrieved due to a hardware error.
  @retval EFI_UNSUPPORTED       After ExitBootServices() has been called, this return code may be returned
                                if no variable storage is supported. The platform should describe this
                                runtime service as unsupported at runtime via an EFI_RT_PROPERTIES_TABLE
                                configuration table.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_GET_NEXT_VARIABLE_NAME)(
  UINTN                    *VariableNameSize,
  CHAR16                   *VariableName,
  EFI_GUID                 *VendorGuid
  );

/**
  Sets the value of a variable.

  @param[in]  VariableName       A Null-terminated string that is the name of the vendor's variable.
                                 Each VariableName is unique for each VendorGuid. VariableName must
                                 contain 1 or more characters. If VariableName is an empty string,
                                 then EFI_INVALID_PARAMETER is returned.
  @param[in]  VendorGuid         A unique identifier for the vendor.
  @param[in]  Attributes         Attributes bitmask to set for the variable.
  @param[in]  DataSize           The size in bytes of the Data buffer. Unless the EFI_VARIABLE_APPEND_WRITE or
                                 EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS attribute is set, a size of zero
                                 causes the variable to be deleted. When the EFI_VARIABLE_APPEND_WRITE attribute is
                                 set, then a SetVariable() call with a DataSize of zero will not cause any change to
                                 the variable value (the timestamp associated with the variable may be updated however
                                 even if no new data value is provided,see the description of the
                                 EFI_VARIABLE_AUTHENTICATION_2 descriptor below. In this case the DataSize will not
                                 be zero since the EFI_VARIABLE_AUTHENTICATION_2 descriptor will be populated).
  @param[in]  Data               The contents for the variable.

  @retval EFI_SUCCESS            The firmware has successfully stored the variable and its data as
                                 defined by the Attributes.
  @retval EFI_INVALID_PARAMETER  An invalid combination of attribute bits, name, and GUID was supplied, or the
                                 DataSize exceeds the maximum allowed.
  @retval EFI_INVALID_PARAMETER  VariableName is an empty string.
  @retval EFI_OUT_OF_RESOURCES   Not enough storage is available to hold the variable and its data.
  @retval EFI_DEVICE_ERROR       The variable could not be retrieved due to a hardware error.
  @retval EFI_WRITE_PROTECTED    The variable in question is read-only.
  @retval EFI_WRITE_PROTECTED    The variable in question cannot be deleted.
  @retval EFI_SECURITY_VIOLATION The variable could not be written due to EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACESS being set,
                                 but the AuthInfo does NOT pass the validation check carried out by the firmware.

  @retval EFI_NOT_FOUND          The variable trying to be updated or deleted was not found.
  @retval EFI_UNSUPPORTED        This call is not supported by this platform at the time the call is made.
                                 The platform should describe this runtime service as unsupported at runtime
                                 via an EFI_RT_PROPERTIES_TABLE configuration table.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_VARIABLE)(
  CHAR16                       *VariableName,
  EFI_GUID                     *VendorGuid,
  UINT32                       Attributes,
  UINTN                        DataSize,
  VOID                         *Data
  );

/**
  Returns the next high 32 bits of the platform's monotonic counter.

  @param[out]  HighCount        The pointer to returned value.

  @retval EFI_SUCCESS           The next high monotonic count was returned.
  @retval EFI_INVALID_PARAMETER HighCount is NULL.
  @retval EFI_DEVICE_ERROR      The device is not functioning properly.
  @retval EFI_UNSUPPORTED       This call is not supported by this platform at the time the call is made.
                                The platform should describe this runtime service as unsupported at runtime
                                via an EFI_RT_PROPERTIES_TABLE configuration table.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_GET_NEXT_HIGH_MONO_COUNT)(
  UINT32                  *HighCount
  );

/**
  Resets the entire platform.

  @param[in]  ResetType         The type of reset to perform.
  @param[in]  ResetStatus       The status code for the reset.
  @param[in]  DataSize          The size, in bytes, of ResetData.
  @param[in]  ResetData         For a ResetType of EfiResetCold, EfiResetWarm, or
                                EfiResetShutdown the data buffer starts with a Null-terminated
                                string, optionally followed by additional binary data.
                                The string is a description that the caller may use to further
                                indicate the reason for the system reset.
                                For a ResetType of EfiResetPlatformSpecific the data buffer
                                also starts with a Null-terminated string that is followed
                                by an EFI_GUID that describes the specific type of reset to perform.
**/
typedef
VOID
(EFIAPI *EFI_RESET_SYSTEM)(
  EFI_RESET_TYPE           ResetType,
  EFI_STATUS               ResetStatus,
  UINTN                    DataSize,
  VOID                     *ResetData 
  );

/**
  Passes capsules to the firmware with both virtual and physical mapping. Depending on the intended
  consumption, the firmware may process the capsule immediately. If the payload should persist
  across a system reset, the reset value returned from EFI_QueryCapsuleCapabilities must
  be passed into ResetSystem() and will cause the capsule to be processed by the firmware as
  part of the reset process.

  @param[in]  CapsuleHeaderArray Virtual pointer to an array of virtual pointers to the capsules
                                 being passed into update capsule.
  @param[in]  CapsuleCount       Number of pointers to EFI_CAPSULE_HEADER in
                                 CaspuleHeaderArray.
  @param[in]  ScatterGatherList  Physical pointer to a set of
                                 EFI_CAPSULE_BLOCK_DESCRIPTOR that describes the
                                 location in physical memory of a set of capsules.

  @retval EFI_SUCCESS           Valid capsule was passed. If
                                CAPSULE_FLAGS_PERSIT_ACROSS_RESET is not set, the
                                capsule has been successfully processed by the firmware.
  @retval EFI_INVALID_PARAMETER CapsuleSize is NULL, or an incompatible set of flags were
                                set in the capsule header.
  @retval EFI_INVALID_PARAMETER CapsuleCount is 0.
  @retval EFI_DEVICE_ERROR      The capsule update was started, but failed due to a device error.
  @retval EFI_UNSUPPORTED       The capsule type is not supported on this platform.
  @retval EFI_OUT_OF_RESOURCES  When ExitBootServices() has been previously called this error indicates the capsule
                                is compatible with this platform but is not capable of being submitted or processed
                                in runtime. The caller may resubmit the capsule prior to ExitBootServices().
  @retval EFI_OUT_OF_RESOURCES  When ExitBootServices() has not been previously called then this error indicates
                                the capsule is compatible with this platform but there are insufficient resources to process.
  @retval EFI_UNSUPPORTED       This call is not supported by this platform at the time the call is made.
                                The platform should describe this runtime service as unsupported at runtime
                                via an EFI_RT_PROPERTIES_TABLE configuration table.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_UPDATE_CAPSULE)(
  EFI_CAPSULE_HEADER     **CapsuleHeaderArray,
  UINTN                  CapsuleCount,
  EFI_PHYSICAL_ADDRESS   ScatterGatherList
  );

/**
  Returns if the capsule can be supported via UpdateCapsule().

  @param[in]   CapsuleHeaderArray  Virtual pointer to an array of virtual pointers to the capsules
                                   being passed into update capsule.
  @param[in]   CapsuleCount        Number of pointers to EFI_CAPSULE_HEADER in
                                   CaspuleHeaderArray.
  @param[out]  MaxiumCapsuleSize   On output the maximum size that UpdateCapsule() can
                                   support as an argument to UpdateCapsule() via
                                   CapsuleHeaderArray and ScatterGatherList.
  @param[out]  ResetType           Returns the type of reset required for the capsule update.

  @retval EFI_SUCCESS           Valid answer returned.
  @retval EFI_UNSUPPORTED       The capsule type is not supported on this platform, and
                                MaximumCapsuleSize and ResetType are undefined.
  @retval EFI_INVALID_PARAMETER MaximumCapsuleSize is NULL.
  @retval EFI_OUT_OF_RESOURCES  When ExitBootServices() has been previously called this error indicates the capsule
                                is compatible with this platform but is not capable of being submitted or processed
                                in runtime. The caller may resubmit the capsule prior to ExitBootServices().
  @retval EFI_OUT_OF_RESOURCES  When ExitBootServices() has not been previously called then this error indicates
                                the capsule is compatible with this platform but there are insufficient resources to process.
  @retval EFI_UNSUPPORTED       This call is not supported by this platform at the time the call is made.
                                The platform should describe this runtime service as unsupported at runtime
                                via an EFI_RT_PROPERTIES_TABLE configuration table.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_QUERY_CAPSULE_CAPABILITIES)(
  EFI_CAPSULE_HEADER     **CapsuleHeaderArray,
  UINTN                  CapsuleCount,
  UINT64                 *MaximumCapsuleSize,
  EFI_RESET_TYPE         *ResetType
  );

/**
  Returns information about the EFI variables.

  @param[in]   Attributes                   Attributes bitmask to specify the type of variables on
                                            which to return information.
  @param[out]  MaximumVariableStorageSize   On output the maximum size of the storage space
                                            available for the EFI variables associated with the
                                            attributes specified.
  @param[out]  RemainingVariableStorageSize Returns the remaining size of the storage space
                                            available for the EFI variables associated with the
                                            attributes specified.
  @param[out]  MaximumVariableSize          Returns the maximum size of the individual EFI
                                            variables associated with the attributes specified.

  @retval EFI_SUCCESS                  Valid answer returned.
  @retval EFI_INVALID_PARAMETER        An invalid combination of attribute bits was supplied
  @retval EFI_UNSUPPORTED              The attribute is not supported on this platform, and the
                                       MaximumVariableStorageSize,
                                       RemainingVariableStorageSize, MaximumVariableSize
                                       are undefined.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_QUERY_VARIABLE_INFO)(
  UINT32            Attributes,
  UINT64            *MaximumVariableStorageSize,
  UINT64            *RemainingVariableStorageSize,
  UINT64            *MaximumVariableSize
  );

//
// EFI Runtime Services Table
//
#define EFI_SYSTEM_TABLE_SIGNATURE      SIGNATURE_64 ('I','B','I',' ','S','Y','S','T')
#define EFI_2_80_SYSTEM_TABLE_REVISION  ((2 << 16) | (80))
#define EFI_2_70_SYSTEM_TABLE_REVISION  ((2 << 16) | (70))
#define EFI_2_60_SYSTEM_TABLE_REVISION  ((2 << 16) | (60))
#define EFI_2_50_SYSTEM_TABLE_REVISION  ((2 << 16) | (50))
#define EFI_2_40_SYSTEM_TABLE_REVISION  ((2 << 16) | (40))
#define EFI_2_31_SYSTEM_TABLE_REVISION  ((2 << 16) | (31))
#define EFI_2_30_SYSTEM_TABLE_REVISION  ((2 << 16) | (30))
#define EFI_2_20_SYSTEM_TABLE_REVISION  ((2 << 16) | (20))
#define EFI_2_10_SYSTEM_TABLE_REVISION  ((2 << 16) | (10))
#define EFI_2_00_SYSTEM_TABLE_REVISION  ((2 << 16) | (00))
#define EFI_1_10_SYSTEM_TABLE_REVISION  ((1 << 16) | (10))
#define EFI_1_02_SYSTEM_TABLE_REVISION  ((1 << 16) | (02))
#define EFI_SYSTEM_TABLE_REVISION       EFI_2_70_SYSTEM_TABLE_REVISION
#define EFI_SPECIFICATION_VERSION       EFI_SYSTEM_TABLE_REVISION

#define EFI_RUNTIME_SERVICES_SIGNATURE  SIGNATURE_64 ('R','U','N','T','S','E','R','V')
#define EFI_RUNTIME_SERVICES_REVISION   EFI_SPECIFICATION_VERSION


///
/// Data structure that precedes all of the standard EFI table types.
///
typedef struct {
  ///
  /// A 64-bit signature that identifies the type of table that follows.
  /// Unique signatures have been generated for the EFI System Table,
  /// the EFI Boot Services Table, and the EFI Runtime Services Table.
  ///
  UINT64    Signature;
  ///
  /// The revision of the EFI Specification to which this table
  /// conforms. The upper 16 bits of this field contain the major
  /// revision value, and the lower 16 bits contain the minor revision
  /// value. The minor revision values are limited to the range of 00..99.
  ///
  UINT32    Revision;
  ///
  /// The size, in bytes, of the entire table including the EFI_TABLE_HEADER.
  ///
  UINT32    HeaderSize;
  ///
  /// The 32-bit CRC for the entire table. This value is computed by
  /// setting this field to 0, and computing the 32-bit CRC for HeaderSize bytes.
  ///
  UINT32    CRC32;
  ///
  /// Reserved field that must be set to 0.
  ///
  UINT32    Reserved;
} EFI_TABLE_HEADER;

///
/// EFI Runtime Services Table.
///
typedef struct {
  ///
  /// The table header for the EFI Runtime Services Table.
  ///
  EFI_TABLE_HEADER                  Hdr;

  //
  // Time Services
  //
  EFI_GET_TIME                      GetTime;
  EFI_SET_TIME                      SetTime;
  EFI_GET_WAKEUP_TIME               GetWakeupTime;
  EFI_SET_WAKEUP_TIME               SetWakeupTime;

  //
  // Virtual Memory Services
  //
  EFI_SET_VIRTUAL_ADDRESS_MAP       SetVirtualAddressMap;
  EFI_CONVERT_POINTER               ConvertPointer;

  //
  // Variable Services
  //
  EFI_GET_VARIABLE                  GetVariable;
  EFI_GET_NEXT_VARIABLE_NAME        GetNextVariableName;
  EFI_SET_VARIABLE                  SetVariable;

  //
  // Miscellaneous Services
  //
  EFI_GET_NEXT_HIGH_MONO_COUNT      GetNextHighMonotonicCount;
  EFI_RESET_SYSTEM                  ResetSystem;

  //
  // UEFI 2.0 Capsule Services
  //
  EFI_UPDATE_CAPSULE                UpdateCapsule;
  EFI_QUERY_CAPSULE_CAPABILITIES    QueryCapsuleCapabilities;

  //
  // Miscellaneous UEFI 2.0 Service
  //
  EFI_QUERY_VARIABLE_INFO           QueryVariableInfo;
} EFI_RUNTIME_SERVICES;

typedef struct {
  ///
  /// The 128-bit GUID value that uniquely identifies the system configuration table.
  ///
  EFI_GUID    VendorGuid;
  ///
  /// A pointer to the table associated with VendorGuid.
  ///
  VOID        *VendorTable;
} EFI_CONFIGURATION_TABLE;

///
/// EFI System Table
///
typedef struct {
  ///
  /// The table header for the EFI System Table.
  ///
  EFI_TABLE_HEADER                   Hdr;
  ///
  /// A pointer to a null terminated string that identifies the vendor
  /// that produces the system firmware for the platform.
  ///
  CHAR16                             *FirmwareVendor;
  ///
  /// A firmware vendor specific value that identifies the revision
  /// of the system firmware for the platform.
  ///
  UINT32                             FirmwareRevision;
  ///
  /// The handle for the active console input device. This handle must support
  /// EFI_SIMPLE_TEXT_INPUT_PROTOCOL and EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL. If
  /// there is no active console, these protocols must still be present.
  ///
  EFI_HANDLE                         ConsoleInHandle;
  ///
  /// A pointer to the EFI_SIMPLE_TEXT_INPUT_PROTOCOL interface that is
  /// associated with ConsoleInHandle.
  ///
  UINT64                             ConIn;
  ///
  /// The handle for the active console output device. This handle must support the
  /// EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL. If there is no active console, these protocols
  /// must still be present.
  ///
  EFI_HANDLE                         ConsoleOutHandle;
  ///
  /// A pointer to the EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL interface
  /// that is associated with ConsoleOutHandle.
  ///
  UINT64                              ConOut;
  ///
  /// The handle for the active standard error console device.
  /// This handle must support the EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL. If there
  /// is no active console, this protocol must still be present.
  ///
  EFI_HANDLE                         StandardErrorHandle;
  ///
  /// A pointer to the EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL interface
  /// that is associated with StandardErrorHandle.
  ///
  UINT64                             StdErr;
  ///
  /// A pointer to the EFI Runtime Services Table.
  ///
  EFI_RUNTIME_SERVICES               RuntimeServices;
  ///
  /// A pointer to the EFI Boot Services Table.
  ///
  UINT64                             BootServices;
  ///
  /// The number of system configuration tables in the buffer ConfigurationTable.
  ///
  UINTN                              NumberOfTableEntries;
  ///
  /// A pointer to the system configuration tables.
  /// The number of entries in the table is NumberOfTableEntries.
  ///
  EFI_CONFIGURATION_TABLE            *ConfigurationTable;
} EFI_SYSTEM_TABLE;

#define EFI_PART_TYPE_UNUSED_GUID \
  { \
    0x00000000, 0x0000, 0x0000, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } \
  }

#define EFI_PART_TYPE_EFI_SYSTEM_PART_GUID \
  { \
    0xc12a7328, 0xf81f, 0x11d2, {0xba, 0x4b, 0x00, 0xa0, 0xc9, 0x3e, 0xc9, 0x3b } \
  }

#define EFI_PART_TYPE_LEGACY_MBR_GUID \
  { \
    0x024dee41, 0x33e7, 0x11d3, {0x9d, 0x69, 0x00, 0x08, 0xc7, 0x81, 0xf3, 0x9f } \
  }

#define EFI_PART_TYPE_MICROSOFT_BASIC_DATA_GUID \
  { \
    0xEBD0A0A2, 0xB9E5, 0x4433, { 0x87, 0xC0, 0x68, 0xB6, 0xB7, 0x26, 0x99, 0xC7 } \
  }

#define EFI_PART_TYPE_LINUX_SWAP_GUID \
  { \
    0x0657FD6D, 0xA4AB, 0x43C4, { 0x84, 0xE5, 0x09, 0x33, 0xC8, 0x4B, 0x4F, 0x4F } \
  }

#define MBR_SIGNATURE  0xaa55

#define EXTENDED_DOS_PARTITION      0x05
#define EXTENDED_WINDOWS_PARTITION  0x0F

#define MAX_MBR_PARTITIONS  4

#define PMBR_GPT_PARTITION  0xEE
#define EFI_PARTITION       0xEF

#define MBR_SIZE  512

///
/// The primary GUID Partition Table Header must be
/// located in LBA 1 (i.e., the second logical block).
///
#define PRIMARY_PART_HEADER_LBA  1
///
/// EFI Partition Table Signature: "EFI PART".
///
#define EFI_PTAB_HEADER_ID  SIGNATURE_64 ('E','F','I',' ','P','A','R','T')
///
/// Minimum bytes reserve for EFI entry array buffer.
///
#define EFI_GPT_PART_ENTRY_MIN_SIZE  16384

#define EFI_GPT_REVISION 0x00010000

#pragma pack(push, 1)

///
/// MBR Partition Entry
///
typedef struct {
  UINT8    BootIndicator;
  UINT8    StartHead;
  UINT8    StartSector;
  UINT8    StartTrack;
  UINT8    OSIndicator;
  UINT8    EndHead;
  UINT8    EndSector;
  UINT8    EndTrack;
  UINT8    StartingLBA[4];
  UINT8    SizeInLBA[4];
} MBR_PARTITION_RECORD;

///
/// MBR Partition Table
///
typedef struct {
  UINT8                   BootStrapCode[440];
  UINT8                   UniqueMbrSignature[4];
  UINT8                   Unknown[2];
  MBR_PARTITION_RECORD    Partition[MAX_MBR_PARTITIONS];
  UINT16                  Signature;
} MASTER_BOOT_RECORD;

typedef UINT64 EFI_LBA;

///
/// GPT Partition Table Header.
///
typedef struct
{
  ///
  /// The table header for the GPT partition Table.
  /// This header contains EFI_PTAB_HEADER_ID.
  ///
  EFI_TABLE_HEADER    Header;
  ///
  /// The LBA that contains this data structure.
  ///
  EFI_LBA             MyLBA;
  ///
  /// LBA address of the alternate GUID Partition Table Header.
  ///
  EFI_LBA             AlternateLBA;
  ///
  /// The first usable logical block that may be used
  /// by a partition described by a GUID Partition Entry.
  ///
  EFI_LBA             FirstUsableLBA;
  ///
  /// The last usable logical block that may be used
  /// by a partition described by a GUID Partition Entry.
  ///
  EFI_LBA             LastUsableLBA;
  ///
  /// GUID that can be used to uniquely identify the disk.
  ///
  EFI_GUID            DiskGUID;
  ///
  /// The starting LBA of the GUID Partition Entry array.
  ///
  EFI_LBA             PartitionEntryLBA;
  ///
  /// The number of Partition Entries in the GUID Partition Entry array.
  ///
  UINT32              NumberOfPartitionEntries;
  ///
  /// The size, in bytes, of each the GUID Partition
  /// Entry structures in the GUID Partition Entry
  /// array. This field shall be set to a value of 128 x 2^n where n is
  /// an integer greater than or equal to zero (e.g., 128, 256, 512, etc.).
  ///
  UINT32              SizeOfPartitionEntry;
  ///
  /// The CRC32 of the GUID Partition Entry array.
  /// Starts at PartitionEntryLBA and is
  /// computed over a byte length of
  /// NumberOfPartitionEntries * SizeOfPartitionEntry.
  ///
  UINT32              PartitionEntryArrayCRC32;
} EFI_PARTITION_TABLE_HEADER;

///
/// GPT Partition Entry.
///
typedef struct
{
  ///
  /// Unique ID that defines the purpose and type of this Partition. A value of
  /// zero defines that this partition entry is not being used.
  ///
  EFI_GUID    PartitionTypeGUID;
  ///
  /// GUID that is unique for every partition entry. Every partition ever
  /// created will have a unique GUID.
  /// This GUID must be assigned when the GUID Partition Entry is created.
  ///
  EFI_GUID    UniquePartitionGUID;
  ///
  /// Starting LBA of the partition defined by this entry
  ///
  EFI_LBA     StartingLBA;
  ///
  /// Ending LBA of the partition defined by this entry.
  ///
  EFI_LBA     EndingLBA;
  ///
  /// Attribute bits, all bits reserved by UEFI
  /// Bit 0:      If this bit is set, the partition is required for the platform to function. The owner/creator of the
  ///             partition indicates that deletion or modification of the contents can result in loss of platform
  ///             features or failure for the platform to boot or operate. The system cannot function normally if
  ///             this partition is removed, and it should be considered part of the hardware of the system.
  ///             Actions such as running diagnostics, system recovery, or even OS install or boot, could
  ///             potentially stop working if this partition is removed. Unless OS software or firmware
  ///             recognizes this partition, it should never be removed or modified as the UEFI firmware or
  ///             platform hardware may become non-functional.
  /// Bit 1:      If this bit is set, then firmware must not produce an EFI_BLOCK_IO_PROTOCOL device for
  ///             this partition. By not producing an EFI_BLOCK_IO_PROTOCOL partition, file system
  ///             mappings will not be created for this partition in UEFI.
  /// Bit 2:      This bit is set aside to let systems with traditional PC-AT BIOS firmware implementations
  ///             inform certain limited, special-purpose software running on these systems that a GPT
  ///             partition may be bootable. The UEFI boot manager must ignore this bit when selecting
  ///             a UEFI-compliant application, e.g., an OS loader.
  /// Bits 3-47:  Undefined and must be zero. Reserved for expansion by future versions of the UEFI
  ///             specification.
  /// Bits 48-63: Reserved for GUID specific use. The use of these bits will vary depending on the
  ///             PartitionTypeGUID. Only the owner of the PartitionTypeGUID is allowed
  ///             to modify these bits. They must be preserved if Bits 0-47 are modified..
  ///
  UINT64    Attributes;
  ///
  /// Null-terminated name of the partition.
  ///
  CHAR16    PartitionName[36];
} EFI_PARTITION_ENTRY;

#pragma pack(pop)
