cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_VIRTUALALLOCACCESSFAIL_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_VIRTUALALLOCACCESSFAIL_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:VirtualAllocAccessFail.exe>/VirtualAllocAccessFail.pdb>
    )
endif()

file(GLOB_RECURSE APP_VIRTUALALLOCACCESSFAIL_SOURCES src/*.c src/*.yasm)

add_executable(VirtualAllocAccessFail.exe ${APP_VIRTUALALLOCACCESSFAIL_SOURCES})
target_include_directories(VirtualAllocAccessFail.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_VIRTUALALLOCACCESSFAIL_INCLUDES}>)
target_compile_options(VirtualAllocAccessFail.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_VIRTUALALLOCACCESSFAIL_PDB_FLAG})
target_compile_definitions(VirtualAllocAccessFail.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(VirtualAllocAccessFail.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(VirtualAllocAccessFail.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS VirtualAllocAccessFail.exe DESTINATION apps/VirtualAllocAccessFail)
install(FILES $<TARGET_FILE_DIR:VirtualAllocAccessFail.exe>/VirtualAllocAccessFail.pdb DESTINATION apps/VirtualAllocAccessFail OPTIONAL)
