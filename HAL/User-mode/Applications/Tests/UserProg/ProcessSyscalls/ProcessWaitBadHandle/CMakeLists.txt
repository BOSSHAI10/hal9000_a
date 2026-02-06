cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_PROCESSWAITBADHANDLE_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_PROCESSWAITBADHANDLE_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:ProcessWaitBadHandle.exe>/ProcessWaitBadHandle.pdb>
    )
endif()

file(GLOB_RECURSE APP_PROCESSWAITBADHANDLE_SOURCES src/*.c src/*.yasm)

add_executable(ProcessWaitBadHandle.exe ${APP_PROCESSWAITBADHANDLE_SOURCES})
target_include_directories(ProcessWaitBadHandle.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_PROCESSWAITBADHANDLE_INCLUDES}>)
target_compile_options(ProcessWaitBadHandle.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_PROCESSWAITBADHANDLE_PDB_FLAG})
target_compile_definitions(ProcessWaitBadHandle.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(ProcessWaitBadHandle.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(ProcessWaitBadHandle.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS ProcessWaitBadHandle.exe DESTINATION apps/ProcessWaitBadHandle)
install(FILES $<TARGET_FILE_DIR:ProcessWaitBadHandle.exe>/ProcessWaitBadHandle.pdb DESTINATION apps/ProcessWaitBadHandle OPTIONAL)
