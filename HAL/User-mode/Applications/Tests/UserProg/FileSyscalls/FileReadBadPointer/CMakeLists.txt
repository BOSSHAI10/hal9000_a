cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_FILEREADBADPOINTER_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_FILEREADBADPOINTER_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:FileReadBadPointer.exe>/FileReadBadPointer.pdb>
    )
endif()

file(GLOB_RECURSE APP_FILEREADBADPOINTER_SOURCES src/*.c src/*.yasm)

add_executable(FileReadBadPointer.exe ${APP_FILEREADBADPOINTER_SOURCES})
target_include_directories(FileReadBadPointer.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_FILEREADBADPOINTER_INCLUDES}>)
target_compile_options(FileReadBadPointer.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_FILEREADBADPOINTER_PDB_FLAG})
target_compile_definitions(FileReadBadPointer.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(FileReadBadPointer.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(FileReadBadPointer.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS FileReadBadPointer.exe DESTINATION apps/FileReadBadPointer)
install(FILES $<TARGET_FILE_DIR:FileReadBadPointer.exe>/FileReadBadPointer.pdb DESTINATION apps/FileReadBadPointer OPTIONAL)
