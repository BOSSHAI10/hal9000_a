cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_PROCESSCREATEMULTIPLE_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_PROCESSCREATEMULTIPLE_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:ProcessCreateMultiple.exe>/ProcessCreateMultiple.pdb>
    )
endif()

file(GLOB_RECURSE APP_PROCESSCREATEMULTIPLE_SOURCES src/*.c src/*.yasm)

add_executable(ProcessCreateMultiple.exe ${APP_PROCESSCREATEMULTIPLE_SOURCES})
target_include_directories(ProcessCreateMultiple.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_PROCESSCREATEMULTIPLE_INCLUDES}>)
target_compile_options(ProcessCreateMultiple.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_PROCESSCREATEMULTIPLE_PDB_FLAG})
target_compile_definitions(ProcessCreateMultiple.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(ProcessCreateMultiple.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(ProcessCreateMultiple.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS ProcessCreateMultiple.exe DESTINATION apps/ProcessCreateMultiple)
install(FILES $<TARGET_FILE_DIR:ProcessCreateMultiple.exe>/ProcessCreateMultiple.pdb DESTINATION apps/ProcessCreateMultiple OPTIONAL)
