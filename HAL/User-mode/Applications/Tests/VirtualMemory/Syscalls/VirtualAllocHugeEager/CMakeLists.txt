cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_VIRTUALALLOCHUGEEAGER_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_VIRTUALALLOCHUGEEAGER_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:VirtualAllocHugeEager.exe>/VirtualAllocHugeEager.pdb>
    )
endif()

file(GLOB_RECURSE APP_VIRTUALALLOCHUGEEAGER_SOURCES src/*.c src/*.yasm)

add_executable(VirtualAllocHugeEager.exe ${APP_VIRTUALALLOCHUGEEAGER_SOURCES})
target_include_directories(VirtualAllocHugeEager.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_VIRTUALALLOCHUGEEAGER_INCLUDES}>)
target_compile_options(VirtualAllocHugeEager.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_VIRTUALALLOCHUGEEAGER_PDB_FLAG})
target_compile_definitions(VirtualAllocHugeEager.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(VirtualAllocHugeEager.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(VirtualAllocHugeEager.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS VirtualAllocHugeEager.exe DESTINATION apps/VirtualAllocHugeEager)
install(FILES $<TARGET_FILE_DIR:VirtualAllocHugeEager.exe>/VirtualAllocHugeEager.pdb DESTINATION apps/VirtualAllocHugeEager OPTIONAL)
