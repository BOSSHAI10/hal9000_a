cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_FILEREADZERO_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_FILEREADZERO_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:FileReadZero.exe>/FileReadZero.pdb>
    )
endif()

file(GLOB_RECURSE APP_FILEREADZERO_SOURCES src/*.c src/*.yasm)

add_executable(FileReadZero.exe ${APP_FILEREADZERO_SOURCES})
target_include_directories(FileReadZero.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_FILEREADZERO_INCLUDES}>)
target_compile_options(FileReadZero.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_FILEREADZERO_PDB_FLAG})
target_compile_definitions(FileReadZero.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(FileReadZero.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(FileReadZero.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS FileReadZero.exe DESTINATION apps/FileReadZero)
install(FILES $<TARGET_FILE_DIR:FileReadZero.exe>/FileReadZero.pdb DESTINATION apps/FileReadZero OPTIONAL)
