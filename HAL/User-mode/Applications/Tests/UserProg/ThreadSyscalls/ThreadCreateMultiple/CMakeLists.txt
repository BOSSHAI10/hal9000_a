cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_THREADCREATEMULTIPLE_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_THREADCREATEMULTIPLE_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:ThreadCreateMultiple.exe>/ThreadCreateMultiple.pdb>
    )
endif()

file(GLOB_RECURSE APP_THREADCREATEMULTIPLE_SOURCES src/*.c src/*.yasm)

add_executable(ThreadCreateMultiple.exe ${APP_THREADCREATEMULTIPLE_SOURCES})
target_include_directories(ThreadCreateMultiple.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_THREADCREATEMULTIPLE_INCLUDES}>)
target_compile_options(ThreadCreateMultiple.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_THREADCREATEMULTIPLE_PDB_FLAG})
target_compile_definitions(ThreadCreateMultiple.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(ThreadCreateMultiple.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(ThreadCreateMultiple.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS ThreadCreateMultiple.exe DESTINATION apps/ThreadCreateMultiple)
install(FILES $<TARGET_FILE_DIR:ThreadCreateMultiple.exe>/ThreadCreateMultiple.pdb DESTINATION apps/ThreadCreateMultiple OPTIONAL)
