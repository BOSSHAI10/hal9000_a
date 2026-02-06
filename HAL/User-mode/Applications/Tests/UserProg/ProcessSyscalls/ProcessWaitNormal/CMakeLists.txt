cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_PROCESSWAITNORMAL_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_PROCESSWAITNORMAL_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:ProcessWaitNormal.exe>/ProcessWaitNormal.pdb>
    )
endif()

file(GLOB_RECURSE APP_PROCESSWAITNORMAL_SOURCES src/*.c src/*.yasm)

add_executable(ProcessWaitNormal.exe ${APP_PROCESSWAITNORMAL_SOURCES})
target_include_directories(ProcessWaitNormal.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_PROCESSWAITNORMAL_INCLUDES}>)
target_compile_options(ProcessWaitNormal.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_PROCESSWAITNORMAL_PDB_FLAG})
target_compile_definitions(ProcessWaitNormal.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(ProcessWaitNormal.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(ProcessWaitNormal.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS ProcessWaitNormal.exe DESTINATION apps/ProcessWaitNormal)
install(FILES $<TARGET_FILE_DIR:ProcessWaitNormal.exe>/ProcessWaitNormal.pdb DESTINATION apps/ProcessWaitNormal OPTIONAL)
