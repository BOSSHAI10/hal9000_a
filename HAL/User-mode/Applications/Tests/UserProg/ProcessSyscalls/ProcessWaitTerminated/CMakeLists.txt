cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_PROCESSWAITTERMINATED_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_PROCESSWAITTERMINATED_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:ProcessWaitTerminated.exe>/ProcessWaitTerminated.pdb>
    )
endif()

file(GLOB_RECURSE APP_PROCESSWAITTERMINATED_SOURCES src/*.c src/*.yasm)

add_executable(ProcessWaitTerminated.exe ${APP_PROCESSWAITTERMINATED_SOURCES})
target_include_directories(ProcessWaitTerminated.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_PROCESSWAITTERMINATED_INCLUDES}>)
target_compile_options(ProcessWaitTerminated.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_PROCESSWAITTERMINATED_PDB_FLAG})
target_compile_definitions(ProcessWaitTerminated.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(ProcessWaitTerminated.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(ProcessWaitTerminated.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS ProcessWaitTerminated.exe DESTINATION apps/ProcessWaitTerminated)
install(FILES $<TARGET_FILE_DIR:ProcessWaitTerminated.exe>/ProcessWaitTerminated.pdb DESTINATION apps/ProcessWaitTerminated OPTIONAL)
