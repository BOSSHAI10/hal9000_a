if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows" AND NOT FORCE_ELF)
    find_program(CMAKE_YASM_COMPILER
        NAMES "vsyasm"
        HINTS ${CMAKE_SOURCE_DIR}/tools/yasm/bin
        REQUIRED
    )
else()
    find_program(CMAKE_YASM_COMPILER
        NAMES "nasm"
        REQUIRED
    )
endif()
mark_as_advanced(CMAKE_YASM_COMPILER)

set(CMAKE_YASM_COMPILER_ID "YASM")
set(CMAKE_YASM_SOURCE_FILE_EXTENSIONS yasm YASM)
set(CMAKE_YASM_OUTPUT_EXTENSION .obj)
set(CMAKE_YASM_COMPILER_ENV_VAR "YASM")

configure_file(${CMAKE_CURRENT_LIST_DIR}/CMakeYASMCompiler.cmake.in
               ${CMAKE_PLATFORM_INFO_DIR}/CMakeYASMCompiler.cmake
)
