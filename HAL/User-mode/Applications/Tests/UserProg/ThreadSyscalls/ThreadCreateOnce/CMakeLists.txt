cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_THREADCREATEONCE_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_THREADCREATEONCE_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:ThreadCreateOnce.exe>/ThreadCreateOnce.pdb>
    )
endif()

file(GLOB_RECURSE APP_THREADCREATEONCE_SOURCES src/*.c src/*.yasm)

add_executable(ThreadCreateOnce.exe ${APP_THREADCREATEONCE_SOURCES})
target_include_directories(ThreadCreateOnce.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_THREADCREATEONCE_INCLUDES}>)
target_compile_options(ThreadCreateOnce.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_THREADCREATEONCE_PDB_FLAG})
target_compile_definitions(ThreadCreateOnce.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(ThreadCreateOnce.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(ThreadCreateOnce.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS ThreadCreateOnce.exe DESTINATION apps/ThreadCreateOnce)
install(FILES $<TARGET_FILE_DIR:ThreadCreateOnce.exe>/ThreadCreateOnce.pdb DESTINATION apps/ThreadCreateOnce OPTIONAL)
