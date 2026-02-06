cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_FILEREADNORMAL_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_FILEREADNORMAL_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:FileReadNormal.exe>/FileReadNormal.pdb>
    )
endif()

file(GLOB_RECURSE APP_FILEREADNORMAL_SOURCES src/*.c src/*.yasm)

add_executable(FileReadNormal.exe ${APP_FILEREADNORMAL_SOURCES})
target_include_directories(FileReadNormal.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_FILEREADNORMAL_INCLUDES}>)
target_compile_options(FileReadNormal.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_FILEREADNORMAL_PDB_FLAG})
target_compile_definitions(FileReadNormal.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(FileReadNormal.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(FileReadNormal.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS FileReadNormal.exe DESTINATION apps/FileReadNormal)
install(FILES $<TARGET_FILE_DIR:FileReadNormal.exe>/FileReadNormal.pdb DESTINATION apps/FileReadNormal OPTIONAL)
