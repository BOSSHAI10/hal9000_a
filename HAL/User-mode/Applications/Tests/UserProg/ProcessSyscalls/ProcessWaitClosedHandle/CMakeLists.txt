cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_PROCESSWAITCLOSEDHANDLE_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_PROCESSWAITCLOSEDHANDLE_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:ProcessWaitClosedHandle.exe>/ProcessWaitClosedHandle.pdb>
    )
endif()

file(GLOB_RECURSE APP_PROCESSWAITCLOSEDHANDLE_SOURCES src/*.c src/*.yasm)

add_executable(ProcessWaitClosedHandle.exe ${APP_PROCESSWAITCLOSEDHANDLE_SOURCES})
target_include_directories(ProcessWaitClosedHandle.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_PROCESSWAITCLOSEDHANDLE_INCLUDES}>)
target_compile_options(ProcessWaitClosedHandle.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_PROCESSWAITCLOSEDHANDLE_PDB_FLAG})
target_compile_definitions(ProcessWaitClosedHandle.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(ProcessWaitClosedHandle.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(ProcessWaitClosedHandle.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS ProcessWaitClosedHandle.exe DESTINATION apps/ProcessWaitClosedHandle)
install(FILES $<TARGET_FILE_DIR:ProcessWaitClosedHandle.exe>/ProcessWaitClosedHandle.pdb DESTINATION apps/ProcessWaitClosedHandle OPTIONAL)
