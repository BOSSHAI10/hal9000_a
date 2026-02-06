cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_THREADCREATEWITHARGUMENTS_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_THREADCREATEWITHARGUMENTS_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:ThreadCreateWithArguments.exe>/ThreadCreateWithArguments.pdb>
    )
endif()

file(GLOB_RECURSE APP_THREADCREATEWITHARGUMENTS_SOURCES src/*.c src/*.yasm)

add_executable(ThreadCreateWithArguments.exe ${APP_THREADCREATEWITHARGUMENTS_SOURCES})
target_include_directories(ThreadCreateWithArguments.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_THREADCREATEWITHARGUMENTS_INCLUDES}>)
target_compile_options(ThreadCreateWithArguments.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_THREADCREATEWITHARGUMENTS_PDB_FLAG})
target_compile_definitions(ThreadCreateWithArguments.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(ThreadCreateWithArguments.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(ThreadCreateWithArguments.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS ThreadCreateWithArguments.exe DESTINATION apps/ThreadCreateWithArguments)
install(FILES $<TARGET_FILE_DIR:ThreadCreateWithArguments.exe>/ThreadCreateWithArguments.pdb DESTINATION apps/ThreadCreateWithArguments OPTIONAL)
