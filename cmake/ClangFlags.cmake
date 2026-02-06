set(GLOBAL_COMMON_DEFINES
    X64
    AMD64
    WIN64
    __HAL9000__
)

set(GLOBAL_DEBUG_DEFINES
    DEBUG
    _DEBUG
)

set(UNICODE_C_DEFINES
    UNICODE
    _UNICODE
)

set(UNICODE_DEFINES
    $<$<COMPILE_LANGUAGE:C>:${UNICODE_C_DEFINES}>
)

set(MBCS_DEFINE
    $<$<COMPILE_LANGUAGE:C>:_MBCS>
)

set(GLOBAL_RELEASE_DEFINES
    NDEBUG
    _NDEBUG
)

set(GLOBAL_C_DEFINES
    ${GLOBAL_COMMON_DEFINES}
    $<$<CONFIG:Debug>:${GLOBAL_DEBUG_DEFINES}>
    $<$<CONFIG:Release>:${GLOBAL_RELEASE_DEFINES}>
)

set(GLOBAL_DEFINES
    $<$<COMPILE_LANGUAGE:C>:${GLOBAL_C_DEFINES}>
)

set(GLOBAL_COMMON_FLAGS
    -fno-zero-initialized-in-bss
    -ffreestanding
    -nostdlib
    -mno-red-zone
    -mgeneral-regs-only
    -fshort-wchar
    -nostdinc
    -mcmodel=large
)

set(GLOBAL_DEBUG_FLAGS 
    -g    
    -MP
    -O0
    -fstack-protector-all
    # -fstack-clash-protection
    -Werror
    -Wall
    -Wextra
)

set(GLOBAL_RELEASE_FLAGS
    -g
    -ffunction-sections
    -MP
    -O2
    -fstack-protector-all
    # -fstack-clash-protection
    -Werror
    -Wall
)

set(GLOBAL_WARNING_DISABLES
    -Wno-unused-but-set-variable
    -Wno-unused-label
    -Wno-missing-field-initializers
    -Wno-multichar
    -Wno-unneeded-internal-declaration
    -Wno-unused-function
)

set(GLOBAL_C_FLAGS
    ${GLOBAL_COMMON_FLAGS}
    $<$<CONFIG:Debug>:${GLOBAL_DEBUG_FLAGS}>
    $<$<CONFIG:Release>:${GLOBAL_RELEASE_FLAGS}>
    ${GLOBAL_WARNING_DISABLES}
)

set(GLOBAL_FLAGS
    $<$<COMPILE_LANGUAGE:C>:${GLOBAL_C_FLAGS}>
)

set(UEFI_APP_C_COMPILE_FLAGS
    -Werror
    -nostdinc
    -ffreestanding
    -nostdlib
    -fshort-wchar
    # -mno-red-zone
)

set(UEFI_APP_COMPILE_FLAGS
    $<$<COMPILE_LANGUAGE:C>:${UEFI_APP_C_COMPILE_FLAGS}>
)

set(UEFI_APP_C_LINK_FLAGS
    # -nostdlib
    # -nodefaultlibs
    # -Wl,-dll
    # -shared
    # -e EfiEntry
    # -Wl,--subsystem,10
    # -Werror
    /ENTRY:EfiEntry
    /SUBSYSTEM:EFI_APPLICATION
    /BASE:0x400000

)

set(UEFI_APP_LINK_FLAGS
    $<$<COMPILE_LANGUAGE:C>:${UEFI_APP_C_LINK_FLAGS}>
)
