cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_FILECREATENORMAL_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_FILECREATENORMAL_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:FileCreateNormal.exe>/FileCreateNormal.pdb>
    )
endif()

file(GLOB_RECURSE APP_FILECREATENORMAL_SOURCES src/*.c src/*.yasm)

add_executable(FileCreateNormal.exe ${APP_FILECREATENORMAL_SOURCES})
target_include_directories(FileCreateNormal.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_FILECREATENORMAL_INCLUDES}>)
target_compile_options(FileCreateNormal.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_FILECREATENORMAL_PDB_FLAG})
target_compile_definitions(FileCreateNormal.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(FileCreateNormal.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(FileCreateNormal.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS FileCreateNormal.exe DESTINATION apps/FileCreateNormal)
install(FILES $<TARGET_FILE_DIR:FileCreateNormal.exe>/FileCreateNormal.pdb DESTINATION apps/FileCreateNormal OPTIONAL)
