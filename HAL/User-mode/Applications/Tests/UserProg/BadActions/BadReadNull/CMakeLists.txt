cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_BADREADNULL_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_BADREADNULL_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:BadReadNull.exe>/BadReadNull.pdb>
    )
endif()

file(GLOB_RECURSE APP_BADREADNULL_SOURCES src/*.c src/*.yasm)

add_executable(BadReadNull.exe ${APP_BADREADNULL_SOURCES})
target_include_directories(BadReadNull.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_BADREADNULL_INCLUDES}>)
target_compile_options(BadReadNull.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_BADREADNULL_PDB_FLAG})
target_compile_definitions(BadReadNull.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(BadReadNull.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(BadReadNull.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS BadReadNull.exe DESTINATION apps/BadReadNull)
install(FILES $<TARGET_FILE_DIR:BadReadNull.exe>/BadReadNull.pdb DESTINATION apps/BadReadNull OPTIONAL)
