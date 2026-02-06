cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_PROCESSCLOSEFILE_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_PROCESSCLOSEFILE_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:ProcessCloseFile.exe>/ProcessCloseFile.pdb>
    )
endif()

file(GLOB_RECURSE APP_PROCESSCLOSEFILE_SOURCES src/*.c src/*.yasm)

add_executable(ProcessCloseFile.exe ${APP_PROCESSCLOSEFILE_SOURCES})
target_include_directories(ProcessCloseFile.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_PROCESSCLOSEFILE_INCLUDES}>)
target_compile_options(ProcessCloseFile.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_PROCESSCLOSEFILE_PDB_FLAG})
target_compile_definitions(ProcessCloseFile.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(ProcessCloseFile.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(ProcessCloseFile.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS ProcessCloseFile.exe DESTINATION apps/ProcessCloseFile)
install(FILES $<TARGET_FILE_DIR:ProcessCloseFile.exe>/ProcessCloseFile.pdb DESTINATION apps/ProcessCloseFile OPTIONAL)
