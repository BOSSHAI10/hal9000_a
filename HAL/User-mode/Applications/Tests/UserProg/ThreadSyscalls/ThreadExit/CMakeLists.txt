cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_THREADEXIT_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_THREADEXIT_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:ThreadExit.exe>/ThreadExit.pdb>
    )
endif()

file(GLOB_RECURSE APP_THREADEXIT_SOURCES src/*.c src/*.yasm)

add_executable(ThreadExit.exe ${APP_THREADEXIT_SOURCES})
target_include_directories(ThreadExit.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_THREADEXIT_INCLUDES}>)
target_compile_options(ThreadExit.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_THREADEXIT_PDB_FLAG})
target_compile_definitions(ThreadExit.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(ThreadExit.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(ThreadExit.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS ThreadExit.exe DESTINATION apps/ThreadExit)
install(FILES $<TARGET_FILE_DIR:ThreadExit.exe>/ThreadExit.pdb DESTINATION apps/ThreadExit OPTIONAL)
