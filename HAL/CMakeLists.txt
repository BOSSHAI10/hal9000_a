cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

MESSAGE(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")

project(HAL9000 LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../cmake/yasm")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/../cmake")
enable_language(YASM)

include(Flags)

set(HAL9000_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/HAL9000/headers/apps
    ${CMAKE_CURRENT_LIST_DIR}/HAL9000/headers/base
    ${CMAKE_CURRENT_LIST_DIR}/HAL9000/headers/boot
    ${CMAKE_CURRENT_LIST_DIR}/HAL9000/headers/core
    ${CMAKE_CURRENT_LIST_DIR}/HAL9000/headers/core/cpu
    ${CMAKE_CURRENT_LIST_DIR}/HAL9000/headers/core/io
    ${CMAKE_CURRENT_LIST_DIR}/HAL9000/headers/core/memory
    ${CMAKE_CURRENT_LIST_DIR}/HAL9000/headers/crt
    ${CMAKE_CURRENT_LIST_DIR}/HAL9000/headers/debug
    ${CMAKE_CURRENT_LIST_DIR}/HAL9000/headers/debug/dump
    ${CMAKE_CURRENT_LIST_DIR}/HAL9000/headers/debug/test
    ${CMAKE_CURRENT_LIST_DIR}/HAL9000/headers/debug/test/threads
    ${CMAKE_CURRENT_LIST_DIR}/HAL9000/headers/debug/test/userprog
    ${CMAKE_CURRENT_LIST_DIR}/HAL9000/headers/devices/apic
    ${CMAKE_CURRENT_LIST_DIR}/HAL9000/headers/devices/keyboard
    ${CMAKE_CURRENT_LIST_DIR}/HAL9000/headers/devices/serial
    ${CMAKE_CURRENT_LIST_DIR}/HAL9000/headers/executive
    ${CMAKE_CURRENT_LIST_DIR}/HAL9000/headers/usermode
)

add_subdirectory(CompilerAbstraction)

add_subdirectory(CommonLib)

add_subdirectory(Filesystems/FAT32)
add_subdirectory(Filesystems/SwapFS)

add_subdirectory(Misc/ELF_Parser)
add_subdirectory(Misc/PE_Parser)

add_subdirectory(Networking/Eth_82574L)
add_subdirectory(Networking/NetworkPort)
add_subdirectory(Networking/NetworkStack)

add_subdirectory(Storage/Ata)
add_subdirectory(Storage/Disk)
add_subdirectory(Storage/Volume)

add_subdirectory(User-mode/Utils/UsermodeLibrary)
add_subdirectory(User-mode/Applications)

add_subdirectory(ACPI)

add_subdirectory(HAL)
add_subdirectory(HAL9000)
