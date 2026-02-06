cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_FILECREATEEMPTYPATH_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_FILECREATEEMPTYPATH_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:FileCreateEmptyPath.exe>/FileCreateEmptyPath.pdb>
    )
endif()

file(GLOB_RECURSE APP_FILECREATEEMPTYPATH_SOURCES src/*.c src/*.yasm)

add_executable(FileCreateEmptyPath.exe ${APP_FILECREATEEMPTYPATH_SOURCES})
target_include_directories(FileCreateEmptyPath.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_FILECREATEEMPTYPATH_INCLUDES}>)
target_compile_options(FileCreateEmptyPath.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_FILECREATEEMPTYPATH_PDB_FLAG})
target_compile_definitions(FileCreateEmptyPath.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(FileCreateEmptyPath.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(FileCreateEmptyPath.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS FileCreateEmptyPath.exe DESTINATION apps/FileCreateEmptyPath)
install(FILES $<TARGET_FILE_DIR:FileCreateEmptyPath.exe>/FileCreateEmptyPath.pdb DESTINATION apps/FileCreateEmptyPath OPTIONAL)
