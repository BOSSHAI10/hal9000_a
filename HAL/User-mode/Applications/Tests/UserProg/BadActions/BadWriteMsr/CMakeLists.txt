cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_BADWRITEMSR_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_BADWRITEMSR_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:BadWriteMsr.exe>/BadWriteMsr.pdb>
    )
endif()

file(GLOB_RECURSE APP_BADWRITEMSR_SOURCES src/*.c src/*.yasm)

add_executable(BadWriteMsr.exe ${APP_BADWRITEMSR_SOURCES})
target_include_directories(BadWriteMsr.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_BADWRITEMSR_INCLUDES}>)
target_compile_options(BadWriteMsr.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_BADWRITEMSR_PDB_FLAG})
target_compile_definitions(BadWriteMsr.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(BadWriteMsr.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(BadWriteMsr.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS BadWriteMsr.exe DESTINATION apps/BadWriteMsr)
install(FILES $<TARGET_FILE_DIR:BadWriteMsr.exe>/BadWriteMsr.pdb DESTINATION apps/BadWriteMsr OPTIONAL)
