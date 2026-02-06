cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_PROCESSCLOSEPARENTHANDLE_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_PROCESSCLOSEPARENTHANDLE_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:ProcessCloseParentHandle.exe>/ProcessCloseParentHandle.pdb>
    )
endif()

file(GLOB_RECURSE APP_PROCESSCLOSEPARENTHANDLE_SOURCES src/*.c src/*.yasm)

add_executable(ProcessCloseParentHandle.exe ${APP_PROCESSCLOSEPARENTHANDLE_SOURCES})
target_include_directories(ProcessCloseParentHandle.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_PROCESSCLOSEPARENTHANDLE_INCLUDES}>)
target_compile_options(ProcessCloseParentHandle.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_PROCESSCLOSEPARENTHANDLE_PDB_FLAG})
target_compile_definitions(ProcessCloseParentHandle.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(ProcessCloseParentHandle.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(ProcessCloseParentHandle.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS ProcessCloseParentHandle.exe DESTINATION apps/ProcessCloseParentHandle)
install(FILES $<TARGET_FILE_DIR:ProcessCloseParentHandle.exe>/ProcessCloseParentHandle.pdb DESTINATION apps/ProcessCloseParentHandle OPTIONAL)
