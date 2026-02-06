cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_PROCESSCLOSENORMAL_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_PROCESSCLOSENORMAL_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:ProcessCloseNormal.exe>/ProcessCloseNormal.pdb>
    )
endif()

file(GLOB_RECURSE APP_PROCESSCLOSENORMAL_SOURCES src/*.c src/*.yasm)

add_executable(ProcessCloseNormal.exe ${APP_PROCESSCLOSENORMAL_SOURCES})
target_include_directories(ProcessCloseNormal.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_PROCESSCLOSENORMAL_INCLUDES}>)
target_compile_options(ProcessCloseNormal.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_PROCESSCLOSENORMAL_PDB_FLAG})
target_compile_definitions(ProcessCloseNormal.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(ProcessCloseNormal.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(ProcessCloseNormal.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS ProcessCloseNormal.exe DESTINATION apps/ProcessCloseNormal)
install(FILES $<TARGET_FILE_DIR:ProcessCloseNormal.exe>/ProcessCloseNormal.pdb DESTINATION apps/ProcessCloseNormal OPTIONAL)
