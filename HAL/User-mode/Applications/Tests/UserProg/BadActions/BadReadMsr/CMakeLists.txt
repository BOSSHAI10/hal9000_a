cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_BADREADMSR_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_BADREADMSR_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:BadReadMsr.exe>/BadReadMsr.pdb>
    )
endif()

file(GLOB_RECURSE APP_BADREADMSR_SOURCES src/*.c src/*.yasm)

add_executable(BadReadMsr.exe ${APP_BADREADMSR_SOURCES})
target_include_directories(BadReadMsr.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_BADREADMSR_INCLUDES}>)
target_compile_options(BadReadMsr.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_BADREADMSR_PDB_FLAG})
target_compile_definitions(BadReadMsr.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(BadReadMsr.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(BadReadMsr.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS BadReadMsr.exe DESTINATION apps/BadReadMsr)
install(FILES $<TARGET_FILE_DIR:BadReadMsr.exe>/BadReadMsr.pdb DESTINATION apps/BadReadMsr OPTIONAL)
