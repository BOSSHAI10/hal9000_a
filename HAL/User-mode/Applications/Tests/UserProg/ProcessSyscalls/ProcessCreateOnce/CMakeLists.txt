cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_PROCESSCREATEONCE_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_PROCESSCREATEONCE_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:ProcessCreateOnce.exe>/ProcessCreateOnce.pdb>
    )
endif()

file(GLOB_RECURSE APP_PROCESSCREATEONCE_SOURCES src/*.c src/*.yasm)

add_executable(ProcessCreateOnce.exe ${APP_PROCESSCREATEONCE_SOURCES})
target_include_directories(ProcessCreateOnce.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_PROCESSCREATEONCE_INCLUDES}>)
target_compile_options(ProcessCreateOnce.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_PROCESSCREATEONCE_PDB_FLAG})
target_compile_definitions(ProcessCreateOnce.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(ProcessCreateOnce.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(ProcessCreateOnce.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS ProcessCreateOnce.exe DESTINATION apps/ProcessCreateOnce)
install(FILES $<TARGET_FILE_DIR:ProcessCreateOnce.exe>/ProcessCreateOnce.pdb DESTINATION apps/ProcessCreateOnce OPTIONAL)
