cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_BADREADNONCANONICAL_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_BADREADNONCANONICAL_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:BadReadNonCanonical.exe>/BadReadNonCanonical.pdb>
    )
endif()

file(GLOB_RECURSE APP_BADREADNONCANONICAL_SOURCES src/*.c src/*.yasm)

add_executable(BadReadNonCanonical.exe ${APP_BADREADNONCANONICAL_SOURCES})
target_include_directories(BadReadNonCanonical.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_BADREADNONCANONICAL_INCLUDES}>)
target_compile_options(BadReadNonCanonical.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_BADREADNONCANONICAL_PDB_FLAG})
target_compile_definitions(BadReadNonCanonical.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(BadReadNonCanonical.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(BadReadNonCanonical.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS BadReadNonCanonical.exe DESTINATION apps/BadReadNonCanonical)
install(FILES $<TARGET_FILE_DIR:BadReadNonCanonical.exe>/BadReadNonCanonical.pdb DESTINATION apps/BadReadNonCanonical OPTIONAL)
