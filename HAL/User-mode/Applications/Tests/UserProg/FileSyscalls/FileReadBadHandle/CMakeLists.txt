cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_FILEREADBADHANDLE_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_FILEREADBADHANDLE_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:FileReadBadHandle.exe>/FileReadBadHandle.pdb>
    )
endif()

file(GLOB_RECURSE APP_FILEREADBADHANDLE_SOURCES src/*.c src/*.yasm)

add_executable(FileReadBadHandle.exe ${APP_FILEREADBADHANDLE_SOURCES})
target_include_directories(FileReadBadHandle.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_FILEREADBADHANDLE_INCLUDES}>)
target_compile_options(FileReadBadHandle.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_FILEREADBADHANDLE_PDB_FLAG})
target_compile_definitions(FileReadBadHandle.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(FileReadBadHandle.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(FileReadBadHandle.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS FileReadBadHandle.exe DESTINATION apps/FileReadBadHandle)
install(FILES $<TARGET_FILE_DIR:FileReadBadHandle.exe>/FileReadBadHandle.pdb DESTINATION apps/FileReadBadHandle OPTIONAL)
