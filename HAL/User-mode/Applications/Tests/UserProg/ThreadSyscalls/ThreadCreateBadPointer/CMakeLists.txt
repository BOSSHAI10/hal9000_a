cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_THREADCREATEBADPOINTER_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_THREADCREATEBADPOINTER_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:ThreadCreateBadPointer.exe>/ThreadCreateBadPointer.pdb>
    )
endif()

file(GLOB_RECURSE APP_THREADCREATEBADPOINTER_SOURCES src/*.c src/*.yasm)

add_executable(ThreadCreateBadPointer.exe ${APP_THREADCREATEBADPOINTER_SOURCES})
target_include_directories(ThreadCreateBadPointer.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_THREADCREATEBADPOINTER_INCLUDES}>)
target_compile_options(ThreadCreateBadPointer.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_THREADCREATEBADPOINTER_PDB_FLAG})
target_compile_definitions(ThreadCreateBadPointer.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(ThreadCreateBadPointer.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(ThreadCreateBadPointer.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS ThreadCreateBadPointer.exe DESTINATION apps/ThreadCreateBadPointer)
install(FILES $<TARGET_FILE_DIR:ThreadCreateBadPointer.exe>/ThreadCreateBadPointer.pdb DESTINATION apps/ThreadCreateBadPointer OPTIONAL)
