cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_DEBUG_FLAGS
        /fp:except-
        /Gy-
    )

    set(APP_RELEASE_FLAGS
        /fp:except-
        /Gy
        /GL
    )

    set(APP_C_LINK_FLAGS
        /ALIGN:4096
        /FILEALIGN:4096
        /IGNORE:4108
        /NXCOMPAT
        /INCREMENTAL:NO
        /ENTRY:__start
        /ERRORREPORT:PROMPT
        /FIXED
        /MANIFEST:NO
        /NODEFAULTLIB
        /DYNAMICBASE:NO
        /SUBSYSTEM:NATIVE
        /NOLOGO
        /MACHINE:X64
        /WX
        /DEBUG:FULL
    )
else()
    set(APP_DEBUG_FLAGS)

    set(APP_RELEASE_FLAGS
        -ffunction-sections
    )

    set(APP_C_LINK_FLAGS
        -static
        # -ffreestanding
        -nostdlib
        # -nodefaultlibs
        # -Wl,--gc-sections
        # -Wall
        # -Wextra
        # -fno-zero-initialized-in-bss
        -T ${CMAKE_CURRENT_LIST_DIR}/Applications.ld
    )
endif()

set(APP_FLAGS
    ${GLOBAL_FLAGS}
    $<$<CONFIG:Debug>:${APP_DEBUG_FLAGS}>
    $<$<CONFIG:Release>:${APP_RELEASE_FLAGS}>
)

set(APP_LINK_FLAGS
    $<$<COMPILE_LANGUAGE:C>:${APP_C_LINK_FLAGS}>
    $<$<NOT:$<STREQUAL:"GNU",$<C_COMPILER_ID>>>:$<$<CONFIG:Release>:/LTCG>>
)

add_subdirectory(AppHelloWorld)
add_subdirectory(TestApp)
add_subdirectory(Tests)
add_subdirectory(LightProjectApp)
