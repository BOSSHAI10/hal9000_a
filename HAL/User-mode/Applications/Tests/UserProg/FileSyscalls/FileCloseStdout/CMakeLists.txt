cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_FILECLOSESTDOUT_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_FILECLOSESTDOUT_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:FileCloseStdout.exe>/FileCloseStdout.pdb>
    )
endif()

file(GLOB_RECURSE APP_FILECLOSESTDOUT_SOURCES src/*.c src/*.yasm)

add_executable(FileCloseStdout.exe ${APP_FILECLOSESTDOUT_SOURCES})
target_include_directories(FileCloseStdout.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_FILECLOSESTDOUT_INCLUDES}>)
target_compile_options(FileCloseStdout.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_FILECLOSESTDOUT_PDB_FLAG})
target_compile_definitions(FileCloseStdout.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(FileCloseStdout.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(FileCloseStdout.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS FileCloseStdout.exe DESTINATION apps/FileCloseStdout)
install(FILES $<TARGET_FILE_DIR:FileCloseStdout.exe>/FileCloseStdout.pdb DESTINATION apps/FileCloseStdout OPTIONAL)
