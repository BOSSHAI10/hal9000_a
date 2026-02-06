cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_THREADWAITCLOSEDHANDLE_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_THREADWAITCLOSEDHANDLE_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:ThreadWaitClosedHandle.exe>/ThreadWaitClosedHandle.pdb>
    )
endif()

file(GLOB_RECURSE APP_THREADWAITCLOSEDHANDLE_SOURCES src/*.c src/*.yasm)

add_executable(ThreadWaitClosedHandle.exe ${APP_THREADWAITCLOSEDHANDLE_SOURCES})
target_include_directories(ThreadWaitClosedHandle.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_THREADWAITCLOSEDHANDLE_INCLUDES}>)
target_compile_options(ThreadWaitClosedHandle.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_THREADWAITCLOSEDHANDLE_PDB_FLAG})
target_compile_definitions(ThreadWaitClosedHandle.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(ThreadWaitClosedHandle.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(ThreadWaitClosedHandle.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS ThreadWaitClosedHandle.exe DESTINATION apps/ThreadWaitClosedHandle)
install(FILES $<TARGET_FILE_DIR:ThreadWaitClosedHandle.exe>/ThreadWaitClosedHandle.pdb DESTINATION apps/ThreadWaitClosedHandle OPTIONAL)
