cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_PROCESSCREATEBADPOINTER_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_PROCESSCREATEBADPOINTER_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:ProcessCreateBadPointer.exe>/ProcessCreateBadPointer.pdb>
    )
endif()

file(GLOB_RECURSE APP_PROCESSCREATEBADPOINTER_SOURCES src/*.c src/*.yasm)

add_executable(ProcessCreateBadPointer.exe ${APP_PROCESSCREATEBADPOINTER_SOURCES})
target_include_directories(ProcessCreateBadPointer.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_PROCESSCREATEBADPOINTER_INCLUDES}>)
target_compile_options(ProcessCreateBadPointer.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_PROCESSCREATEBADPOINTER_PDB_FLAG})
target_compile_definitions(ProcessCreateBadPointer.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(ProcessCreateBadPointer.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(ProcessCreateBadPointer.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS ProcessCreateBadPointer.exe DESTINATION apps/ProcessCreateBadPointer)
install(FILES $<TARGET_FILE_DIR:ProcessCreateBadPointer.exe>/ProcessCreateBadPointer.pdb DESTINATION apps/ProcessCreateBadPointer OPTIONAL)
