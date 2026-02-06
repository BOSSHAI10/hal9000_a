cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_FILECLOSETWICE_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_FILECLOSETWICE_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:FileCloseTwice.exe>/FileCloseTwice.pdb>
    )
endif()

file(GLOB_RECURSE APP_FILECLOSETWICE_SOURCES src/*.c src/*.yasm)

add_executable(FileCloseTwice.exe ${APP_FILECLOSETWICE_SOURCES})
target_include_directories(FileCloseTwice.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_FILECLOSETWICE_INCLUDES}>)
target_compile_options(FileCloseTwice.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_FILECLOSETWICE_PDB_FLAG})
target_compile_definitions(FileCloseTwice.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(FileCloseTwice.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(FileCloseTwice.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS FileCloseTwice.exe DESTINATION apps/FileCloseTwice)
install(FILES $<TARGET_FILE_DIR:FileCloseTwice.exe>/FileCloseTwice.pdb DESTINATION apps/FileCloseTwice OPTIONAL)
