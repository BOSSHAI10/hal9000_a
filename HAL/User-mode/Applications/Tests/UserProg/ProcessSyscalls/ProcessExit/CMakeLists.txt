cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_PROCESSEXIT_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_PROCESSEXIT_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:ProcessExit.exe>/ProcessExit.pdb>
    )
endif()

file(GLOB_RECURSE APP_PROCESSEXIT_SOURCES src/*.c src/*.yasm)

add_executable(ProcessExit.exe ${APP_PROCESSEXIT_SOURCES})
target_include_directories(ProcessExit.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_PROCESSEXIT_INCLUDES}>)
target_compile_options(ProcessExit.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_PROCESSEXIT_PDB_FLAG})
target_compile_definitions(ProcessExit.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(ProcessExit.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(ProcessExit.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS ProcessExit.exe DESTINATION apps/ProcessExit)
install(FILES $<TARGET_FILE_DIR:ProcessExit.exe>/ProcessExit.pdb DESTINATION apps/ProcessExit OPTIONAL)
