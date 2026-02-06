cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_FILECREATEMISSING_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_FILECREATEMISSING_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:FileCreateMissing.exe>/FileCreateMissing.pdb>
    )
endif()

file(GLOB_RECURSE APP_FILECREATEMISSING_SOURCES src/*.c src/*.yasm)

add_executable(FileCreateMissing.exe ${APP_FILECREATEMISSING_SOURCES})
target_include_directories(FileCreateMissing.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_FILECREATEMISSING_INCLUDES}>)
target_compile_options(FileCreateMissing.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_FILECREATEMISSING_PDB_FLAG})
target_compile_definitions(FileCreateMissing.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(FileCreateMissing.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(FileCreateMissing.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS FileCreateMissing.exe DESTINATION apps/FileCreateMissing)
install(FILES $<TARGET_FILE_DIR:FileCreateMissing.exe>/FileCreateMissing.pdb DESTINATION apps/FileCreateMissing OPTIONAL)
