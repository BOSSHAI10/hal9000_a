cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_BADWRITENULL_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_BADWRITENULL_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:BadWriteNull.exe>/BadWriteNull.pdb>
    )
endif()

file(GLOB_RECURSE APP_BADWRITENULL_SOURCES src/*.c src/*.yasm)

add_executable(BadWriteNull.exe ${APP_BADWRITENULL_SOURCES})
target_include_directories(BadWriteNull.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_BADWRITENULL_INCLUDES}>)
target_compile_options(BadWriteNull.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_BADWRITENULL_PDB_FLAG})
target_compile_definitions(BadWriteNull.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(BadWriteNull.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(BadWriteNull.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS BadWriteNull.exe DESTINATION apps/BadWriteNull)
install(FILES $<TARGET_FILE_DIR:BadWriteNull.exe>/BadWriteNull.pdb DESTINATION apps/BadWriteNull OPTIONAL)
