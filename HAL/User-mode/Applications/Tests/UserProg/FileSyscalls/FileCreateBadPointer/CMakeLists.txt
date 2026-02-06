cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_FILECREATEBADPOINTER_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_FILECREATEBADPOINTER_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:FileCreateBadPointer.exe>/FileCreateBadPointer.pdb>
    )
endif()

file(GLOB_RECURSE APP_FILECREATEBADPOINTER_SOURCES src/*.c src/*.yasm)

add_executable(FileCreateBadPointer.exe ${APP_FILECREATEBADPOINTER_SOURCES})
target_include_directories(FileCreateBadPointer.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_FILECREATEBADPOINTER_INCLUDES}>)
target_compile_options(FileCreateBadPointer.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_FILECREATEBADPOINTER_PDB_FLAG})
target_compile_definitions(FileCreateBadPointer.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(FileCreateBadPointer.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(FileCreateBadPointer.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS FileCreateBadPointer.exe DESTINATION apps/FileCreateBadPointer)
install(FILES $<TARGET_FILE_DIR:FileCreateBadPointer.exe>/FileCreateBadPointer.pdb DESTINATION apps/FileCreateBadPointer OPTIONAL)
