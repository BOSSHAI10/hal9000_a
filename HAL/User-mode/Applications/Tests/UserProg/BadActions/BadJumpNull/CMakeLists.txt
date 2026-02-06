cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_BADJUMPNULL_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_BADJUMPNULL_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:BadJumpNull.exe>/BadJumpNull.pdb>
    )
endif()

file(GLOB_RECURSE APP_BADJUMPNULL_SOURCES src/*.c src/*.yasm)

add_executable(BadJumpNull.exe ${APP_BADJUMPNULL_SOURCES})
target_include_directories(BadJumpNull.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_BADJUMPNULL_INCLUDES}>)
target_compile_options(BadJumpNull.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_BADJUMPNULL_PDB_FLAG})
target_compile_definitions(BadJumpNull.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(BadJumpNull.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(BadJumpNull.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS BadJumpNull.exe DESTINATION apps/BadJumpNull)
install(FILES $<TARGET_FILE_DIR:BadJumpNull.exe>/BadJumpNull.pdb DESTINATION apps/BadJumpNull OPTIONAL)
