cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(ACPI_INCLUDES
    ${CMAKE_CURRENT_LIST_DIR}/inc
    ${CMAKE_CURRENT_LIST_DIR}/inc/platform
)

set(ACPI_DEFINES
    ACPI_LIBRARY
    NDEBUG
    _GEN
    DRIVER
    _WINDOWS
    PROCESSOR_ARCHITECTURE=x64
    WIN64
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(ACPI_FLAGS
        /Zi 
        /Fd:$<TARGET_FILE_DIR:Acpica>/Acpica.pdb
        /nologo
        /W4
        /WX-
        /diagnostics:column
        /O1
        /Ob1
        /Os
        /GF
        /Gm-
        /MT
        /GS
        /Gy
        /fp:precise
        /Za
        /Zc:wchar_t
        /Zc:forScope
        /Zc:inline
        /FR
        /Gr
        /TC
        /FC
        /errorReport:prompt
        /FS
    )

    set(ACPI_LINK_FLAGS
        /DEBUG
        /NOLOGO
        /MACHINE:X64
    )

    set(ACPI_IGNORE_WARNINGS
        /wd4295
        /wd4244
        /wd4715
    )
else()
    list(APPEND ACPI_DEFINES __HAL9000__)

    set(ACPI_FLAGS
        -g 
        # Not our code
        # -Wall
        # -Wextra
        -w
        -O1
        -MT
        -fstack-protector-all
        # -fstack-clash-protection
        -ffunction-sections
        -fshort-wchar
        -fno-zero-initialized-in-bss
        -mcmodel=large
    )

    set(ACPI_LINK_FLAGS)

    set(ACPI_IGNORE_WARNINGS)
endif()

file(GLOB_RECURSE ACPI_DEBUGGER_SOURCES src/Debugger/*.c)
file(GLOB_RECURSE ACPI_DISASSEMBLER_SOURCES src/Disassembler/*.c)
file(GLOB_RECURSE ACPI_DISPATCHER_SOURCES src/Dispatcher/*.c)
file(GLOB_RECURSE ACPI_EVENTS_SOURCES src/Events/*.c)
file(GLOB_RECURSE ACPI_EXECUTER_SOURCES src/Executer/*.c)
file(GLOB_RECURSE ACPI_HARDWARE_SOURCES src/Hardware/*.c)
file(GLOB_RECURSE ACPI_NAMESPACE_SOURCES src/Namespace/*.c)
file(GLOB_RECURSE ACPI_PARSER_SOURCES src/Parser/*.c)
file(GLOB_RECURSE ACPI_RESOURCES_SOURCES src/Resources/*.c)
file(GLOB_RECURSE ACPI_TABLES_SOURCES src/Tables/*.c)
file(GLOB_RECURSE ACPI_UTILITIES_SOURCES src/Utilities/*.c)

set(ACPI_SOURCES "")
list(APPEND ACPI_SOURCES "${ACPI_DEBUGGER_SOURCES}")
list(APPEND ACPI_SOURCES "${ACPI_DISASSEMBLER_SOURCES}")
list(APPEND ACPI_SOURCES "${ACPI_DISPATCHER_SOURCES}")
list(APPEND ACPI_SOURCES "${ACPI_EVENTS_SOURCES}")
list(APPEND ACPI_SOURCES "${ACPI_EXECUTER_SOURCES}")
list(APPEND ACPI_SOURCES "${ACPI_HARDWARE_SOURCES}")
list(APPEND ACPI_SOURCES "${ACPI_NAMESPACE_SOURCES}")
list(APPEND ACPI_SOURCES "${ACPI_PARSER_SOURCES}")
list(APPEND ACPI_SOURCES "${ACPI_RESOURCES_SOURCES}")
list(APPEND ACPI_SOURCES "${ACPI_TABLES_SOURCES}")
list(APPEND ACPI_SOURCES "${ACPI_UTILITIES_SOURCES}")

add_library(Acpica STATIC ${ACPI_SOURCES})

target_include_directories(Acpica PUBLIC $<$<COMPILE_LANGUAGE:C>:${ACPI_INCLUDES}>)
target_compile_options(Acpica PRIVATE ${ACPI_FLAGS} PRIVATE ${ACPI_IGNORE_WARNINGS})
target_compile_definitions(Acpica PRIVATE ${ACPI_DEFINES})
target_link_options(Acpica PRIVATE ${ACPI_LINK_FLAGS})

install(TARGETS Acpica DESTINATION lib)
install(FILES $<TARGET_FILE_DIR:Acpica>/Acpica.pdb DESTINATION lib OPTIONAL)
