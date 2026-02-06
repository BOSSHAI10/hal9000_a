cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(HAL9000_C_INCLUDES
    ${CMAKE_SOURCE_DIR}/CompilerAbstraction/inc
    ${CMAKE_CURRENT_LIST_DIR}/headers/apps
    ${CMAKE_CURRENT_LIST_DIR}/headers/base
    ${CMAKE_CURRENT_LIST_DIR}/headers/boot
    ${CMAKE_CURRENT_LIST_DIR}/headers/core
    ${CMAKE_CURRENT_LIST_DIR}/headers/core/cpu
    ${CMAKE_CURRENT_LIST_DIR}/headers/core/io
    ${CMAKE_CURRENT_LIST_DIR}/headers/core/memory
    ${CMAKE_CURRENT_LIST_DIR}/headers/crt
    ${CMAKE_CURRENT_LIST_DIR}/headers/debug
    ${CMAKE_CURRENT_LIST_DIR}/headers/debug/dump
    ${CMAKE_CURRENT_LIST_DIR}/headers/debug/test
    ${CMAKE_CURRENT_LIST_DIR}/headers/debug/test/threads
    ${CMAKE_CURRENT_LIST_DIR}/headers/debug/test/userprog
    ${CMAKE_CURRENT_LIST_DIR}/headers/devices/apic
    ${CMAKE_CURRENT_LIST_DIR}/headers/devices/keyboard
    ${CMAKE_CURRENT_LIST_DIR}/headers/devices/serial
    ${CMAKE_CURRENT_LIST_DIR}/headers/executive
    ${CMAKE_CURRENT_LIST_DIR}/headers/usermode
    ${PROJECT_SOURCE_DIR}/Shared/common
    ${PROJECT_SOURCE_DIR}/Shared/kernel
)
set(HAL9000_YASM_INCLUDES
    ${CMAKE_CURRENT_LIST_DIR}/headers/base/yasm
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(HAL9000_COMMON_FLAGS
        /fp:except-
        /wd4313
        /wd4474
        /wd4476
        /wd4477
    )

    set(HAL9000_DEBUG_FLAGS
        /Gy-
    )

    set(HAL9000_RELEASE_FLAGS 
        /FAs
        /Gy
        /GL
    )

    set(HAL9000_C_FLAGS 
        ${HAL9000_COMMON_FLAGS} 
        $<IF:$<CONFIG:Release>, ${HAL9000_RELEASE_FLAGS}, ${HAL9000_DEBUG_FLAGS}>
    )

    set(HAL9000_FLAGS
        $<$<COMPILE_LANGUAGE:C>:${HAL9000_C_FLAGS}>
    )

    set(HAL9000_COMMON_LINK_FLAGS
        /INCREMENTAL:NO
        /NOLOGO
        /WX
        /NODEFAULTLIB
        /MANIFEST
        /MANIFESTUAC:level='asInvoker'
        /MANIFESTUAC:uiAccess='false'
        /MAPINFO:EXPORTS
        /SUBSYSTEM:NATIVE
        /ENTRY:__HalEntry
        /BASE:0xFFFF800001000000
        /DYNAMICBASE:NO
        /FIXED
        /NXCOMPAT
        /MACHINE:X64
        /ERRORREPORT:PROMPT
        /ALIGN:512
        /IGNORE:4108
        /MERGE:.halboot=.text
        /DEBUG:FULL
    )

    set(HAL9000_MAP_LINK_FLAG
        $<$<COMPILE_LANGUAGE:C>:/MAP:$<TARGET_FILE_DIR:HAL9000.bin>/HAL9000.map>
    )

    set(HAL9000_RELEASE_LINK_FLAGS
        $<$<COMPILE_LANGUAGE:C>:/LTCG>
    )

    set(HAL9000_PDB_LINK_FLAG
        $<$<COMPILE_LANGUAGE:C>:/PDB:$<TARGET_FILE_DIR:HAL9000.bin>/HAL9000.pdb>
    )
else()
    set(HAL9000_COMMON_FLAGS)

    set(HAL9000_DEBUG_FLAGS)

    set(HAL9000_RELEASE_FLAGS)

    set(HAL9000_C_FLAGS 
        ${HAL9000_COMMON_FLAGS} 
        $<$<CONFIG:Debug>:${HAL9000_DEBUG_FLAGS}>
        $<$<CONFIG:Release>:${HAL9000_RELEASE_FLAGS}>
    )

    set(HAL9000_FLAGS
        $<$<COMPILE_LANGUAGE:C>:${HAL9000_C_FLAGS}>
    )

    set(HAL9000_COMMON_LINK_FLAGS
        -g
        -O0
        -static
        # -ffreestanding
        -nostdlib
        # -nodefaultlibs
        # -lgcc
        # /BASE:0xFFFF800001000000
        # /ALIGN:512
        -T ${CMAKE_CURRENT_LIST_DIR}/HAL9000.ld
    )

    set(HAL9000_MAP_LINK_FLAG)

    set(HAL9000_RELEASE_LINK_FLAGS)

    set(HAL9000_PDB_LINK_FLAG)
endif()

set(HAL9000_LINK_FLAGS
    ${HAL9000_COMMON_LINK_FLAGS}
    ${HAL9000_MAP_LINK_FLAG}
    $<$<CONFIG:Release>:${HAL9000_RELEASE_LINK_FLAGS}>
    ${HAL9000_PDB_LINK_FLAG}
)

file(GLOB_RECURSE HAL9000_SOURCES src/*.c src/*.yasm src/_*.yasm)

add_executable(HAL9000.bin ${HAL9000_SOURCES})

target_include_directories(HAL9000.bin 
    PRIVATE $<$<COMPILE_LANGUAGE:C>:${HAL9000_C_INCLUDES}>
    PRIVATE $<$<COMPILE_LANGUAGE:YASM>:${HAL9000_YASM_INCLUDES}>
)
target_compile_options(HAL9000.bin PRIVATE ${GLOBAL_FLAGS} PRIVATE ${HAL9000_FLAGS})
target_compile_definitions(HAL9000.bin PRIVATE ${GLOBAL_DEFINES})
target_link_libraries(HAL9000.bin PRIVATE
    HAL
    FAT32
    SwapFS
    ELF_Parser
    PE_Parser
    Eth_82574L
    NetworkStack
    NetworkPort
    Disk
    Volume
    Ata
    Acpica
    CompilerAbstraction
    CommonLib
)
target_link_options(HAL9000.bin PRIVATE ${HAL9000_LINK_FLAGS})

install(TARGETS HAL9000.bin DESTINATION bin)
install(FILES $<TARGET_FILE_DIR:HAL9000.bin>/HAL9000.pdb DESTINATION bin OPTIONAL)
install(FILES $<TARGET_FILE_DIR:HAL9000.bin>/HAL9000.map DESTINATION bin OPTIONAL)
