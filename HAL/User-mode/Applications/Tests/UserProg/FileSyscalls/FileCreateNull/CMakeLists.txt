cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_FILECREATENULL_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_FILECREATENULL_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:FileCreateNull.exe>/FileCreateNull.pdb>
    )
endif()

file(GLOB_RECURSE APP_FILECREATENULL_SOURCES src/*.c src/*.yasm)

add_executable(FileCreateNull.exe ${APP_FILECREATENULL_SOURCES})
target_include_directories(FileCreateNull.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_FILECREATENULL_INCLUDES}>)
target_compile_options(FileCreateNull.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_FILECREATENULL_PDB_FLAG})
target_compile_definitions(FileCreateNull.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(FileCreateNull.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(FileCreateNull.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS FileCreateNull.exe DESTINATION apps/FileCreateNull)
install(FILES $<TARGET_FILE_DIR:FileCreateNull.exe>/FileCreateNull.pdb DESTINATION apps/FileCreateNull OPTIONAL)
