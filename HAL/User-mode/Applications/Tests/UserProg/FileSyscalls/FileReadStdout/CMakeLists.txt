cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_FILEREADSTDOUT_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_FILEREADSTDOUT_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:FileReadStdout.exe>/FileReadStdout.pdb>
    )
endif()

file(GLOB_RECURSE APP_FILEREADSTDOUT_SOURCES src/*.c src/*.yasm)

add_executable(FileReadStdout.exe ${APP_FILEREADSTDOUT_SOURCES})
target_include_directories(FileReadStdout.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_FILEREADSTDOUT_INCLUDES}>)
target_compile_options(FileReadStdout.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_FILEREADSTDOUT_PDB_FLAG})
target_compile_definitions(FileReadStdout.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(FileReadStdout.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(FileReadStdout.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS FileReadStdout.exe DESTINATION apps/FileReadStdout)
install(FILES $<TARGET_FILE_DIR:FileReadStdout.exe>/FileReadStdout.pdb DESTINATION apps/FileReadStdout OPTIONAL)
