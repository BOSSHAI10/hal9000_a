cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_FILECREATEEXISTENT_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_FILECREATEEXISTENT_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:FileCreateExistent.exe>/FileCreateExistent.pdb>
    )
endif()

file(GLOB_RECURSE APP_FILECREATEEXISTENT_SOURCES src/*.c src/*.yasm)

add_executable(FileCreateExistent.exe ${APP_FILECREATEEXISTENT_SOURCES})
target_include_directories(FileCreateExistent.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_FILECREATEEXISTENT_INCLUDES}>)
target_compile_options(FileCreateExistent.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_FILECREATEEXISTENT_PDB_FLAG})
target_compile_definitions(FileCreateExistent.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(FileCreateExistent.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(FileCreateExistent.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS FileCreateExistent.exe DESTINATION apps/FileCreateExistent)
install(FILES $<TARGET_FILE_DIR:FileCreateExistent.exe>/FileCreateExistent.pdb DESTINATION apps/FileCreateExistent OPTIONAL)
