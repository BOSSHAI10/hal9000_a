cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_FILECLOSENORMAL_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_FILECLOSENORMAL_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:FileCloseNormal.exe>/FileCloseNormal.pdb>
    )
endif()

file(GLOB_RECURSE APP_FILECLOSENORMAL_SOURCES src/*.c src/*.yasm)

add_executable(FileCloseNormal.exe ${APP_FILECLOSENORMAL_SOURCES})
target_include_directories(FileCloseNormal.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_FILECLOSENORMAL_INCLUDES}>)
target_compile_options(FileCloseNormal.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_FILECLOSENORMAL_PDB_FLAG})
target_compile_definitions(FileCloseNormal.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(FileCloseNormal.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(FileCloseNormal.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS FileCloseNormal.exe DESTINATION apps/FileCloseNormal)
install(FILES $<TARGET_FILE_DIR:FileCloseNormal.exe>/FileCloseNormal.pdb DESTINATION apps/FileCloseNormal OPTIONAL)
