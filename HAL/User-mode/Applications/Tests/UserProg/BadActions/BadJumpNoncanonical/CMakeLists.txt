cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_BADJUMPNONCANONICAL_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_BADJUMPNONCANONICAL_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:BadJumpNoncanonical.exe>/BadJumpNoncanonical.pdb>
    )
endif()

file(GLOB_RECURSE APP_BADJUMPNONCANONICAL_SOURCES src/*.c src/*.yasm)

add_executable(BadJumpNoncanonical.exe ${APP_BADJUMPNONCANONICAL_SOURCES})
target_include_directories(BadJumpNoncanonical.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_BADJUMPNONCANONICAL_INCLUDES}>)
target_compile_options(BadJumpNoncanonical.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_BADJUMPNONCANONICAL_PDB_FLAG})
target_compile_definitions(BadJumpNoncanonical.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(BadJumpNoncanonical.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(BadJumpNoncanonical.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS BadJumpNoncanonical.exe DESTINATION apps/BadJumpNoncanonical)
install(FILES $<TARGET_FILE_DIR:BadJumpNoncanonical.exe>/BadJumpNoncanonical.pdb DESTINATION apps/BadJumpNoncanonical OPTIONAL)
