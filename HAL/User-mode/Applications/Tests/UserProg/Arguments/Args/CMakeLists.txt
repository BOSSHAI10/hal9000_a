cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_ARGS_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_ARGS_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:Args.exe>/Args.pdb>
    )
endif()

file(GLOB_RECURSE APP_ARGS_SOURCES src/*.c src/*.yasm)

add_executable(Args.exe ${APP_ARGS_SOURCES})
target_include_directories(Args.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_ARGS_INCLUDES}>)
target_compile_options(Args.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_ARGS_PDB_FLAG})
target_compile_definitions(Args.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(Args.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(Args.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS Args.exe DESTINATION apps/Args)
install(FILES $<TARGET_FILE_DIR:Args.exe>/Args.pdb DESTINATION apps/Args OPTIONAL)
