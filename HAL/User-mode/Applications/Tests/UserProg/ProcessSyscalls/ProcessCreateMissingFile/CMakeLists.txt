cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_PROCESSCREATEMISSINGFILE_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_PROCESSCREATEMISSINGFILE_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:ProcessCreateMissingFile.exe>/ProcessCreateMissingFile.pdb>
    )
endif()

file(GLOB_RECURSE APP_PROCESSCREATEMISSINGFILE_SOURCES src/*.c src/*.yasm)

add_executable(ProcessCreateMissingFile.exe ${APP_PROCESSCREATEMISSINGFILE_SOURCES})
target_include_directories(ProcessCreateMissingFile.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_PROCESSCREATEMISSINGFILE_INCLUDES}>)
target_compile_options(ProcessCreateMissingFile.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_PROCESSCREATEMISSINGFILE_PDB_FLAG})
target_compile_definitions(ProcessCreateMissingFile.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(ProcessCreateMissingFile.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(ProcessCreateMissingFile.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS ProcessCreateMissingFile.exe DESTINATION apps/ProcessCreateMissingFile)
install(FILES $<TARGET_FILE_DIR:ProcessCreateMissingFile.exe>/ProcessCreateMissingFile.pdb DESTINATION apps/ProcessCreateMissingFile OPTIONAL)
