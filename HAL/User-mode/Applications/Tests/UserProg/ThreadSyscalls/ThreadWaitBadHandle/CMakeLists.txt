cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_THREADWAITBADHANDLE_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_THREADWAITBADHANDLE_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:ThreadWaitBadHandle.exe>/ThreadWaitBadHandle.pdb>
    )
endif()

file(GLOB_RECURSE APP_THREADWAITBADHANDLE_SOURCES src/*.c src/*.yasm)

add_executable(ThreadWaitBadHandle.exe ${APP_THREADWAITBADHANDLE_SOURCES})
target_include_directories(ThreadWaitBadHandle.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_THREADWAITBADHANDLE_INCLUDES}>)
target_compile_options(ThreadWaitBadHandle.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_THREADWAITBADHANDLE_PDB_FLAG})
target_compile_definitions(ThreadWaitBadHandle.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(ThreadWaitBadHandle.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(ThreadWaitBadHandle.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS ThreadWaitBadHandle.exe DESTINATION apps/ThreadWaitBadHandle)
install(FILES $<TARGET_FILE_DIR:ThreadWaitBadHandle.exe>/ThreadWaitBadHandle.pdb DESTINATION apps/ThreadWaitBadHandle OPTIONAL)
