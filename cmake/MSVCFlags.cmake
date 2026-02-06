set(GLOBAL_COMMON_DEFINES
    X64
    AMD64
    WIN64
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
    $<IF:$<CONFIG:Release>, ${GLOBAL_RELEASE_DEFINES}, ${GLOBAL_DEBUG_DEFINES}>
)

set(GLOBAL_DEFINES
    $<$<COMPILE_LANGUAGE:C>:${GLOBAL_C_DEFINES}>
)

set(GLOBAL_COMMON_FLAGS
    /FS
    /wd4206
)

set(GLOBAL_DEBUG_FLAGS 
    /RTC1 
    /Gd 
    /Zi
    /diagnostics:column
    /Gm- 
    /Qpar- 
    /GR-
    /GF 
    /fp:precise 
    /Zc:forScope 
    /Ob1 
    /errorReport:prompt
    /MP
    /Oy
    /openmp-
    /Od
    /Zc:inline
    /MTd
    /sdl
    /GS
    /JMC
    /nologo
    /WX
    /Zc:wchar_t
    /FC
    /W4
)

set(GLOBAL_RELEASE_FLAGS
    /Gd 
    /Zi
    /diagnostics:column
    /Gy
    /Oi
    /Gm-
    /Qpar-
    /GR-
    /GF
    /fp:precise
    /Zc:forScope
    /errorReport:prompt
    /MP
    /openmp-
    /O2
    /Zc:inline
    /MTd
    /sdl
    /GS
    /JMC
    /nologo
    /WX
    /Zc:wchar_t
    /FC
    /W4
)

set(GL_RELEASE_FLAG
    $<$<CONFIG:Release>:$<$<COMPILE_LANGUAGE:C>:/GL>>
)

set(GLOBAL_C_FLAGS
    ${GLOBAL_COMMON_FLAGS}
    $<IF:$<CONFIG:Release>, ${GLOBAL_RELEASE_FLAGS}, ${GLOBAL_DEBUG_FLAGS}>
)

set(GLOBAL_FLAGS
    $<$<COMPILE_LANGUAGE:C>:${GLOBAL_C_FLAGS}>
)

set(UEFI_APP_C_COMPILE_FLAGS
    /nologo
    /Gd
    /GF
    /GR-
    /GS-
    /Gs999999999
    /EHs-c-
    /sdl-
    /WX
    /X
)

set(UEFI_APP_COMPILE_FLAGS
    $<$<COMPILE_LANGUAGE:C>:${UEFI_APP_C_COMPILE_FLAGS}>
)

set(UEFI_APP_C_LINK_FLAGS
    /NOLOGO
    /NODEFAULTLIB
    /DLL
    /ENTRY:EfiEntry
    /MACHINE:X64
    /SUBSYSTEM:EFI_APPLICATION
    /WX
)

set(UEFI_APP_LINK_FLAGS
    $<$<COMPILE_LANGUAGE:C>:${UEFI_APP_C_LINK_FLAGS}>
)
