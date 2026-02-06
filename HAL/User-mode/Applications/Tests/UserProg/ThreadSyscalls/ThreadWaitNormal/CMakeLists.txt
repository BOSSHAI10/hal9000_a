cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_THREADWAITNORMAL_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_THREADWAITNORMAL_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:ThreadWaitNormal.exe>/ThreadWaitNormal.pdb>
    )
endif()

file(GLOB_RECURSE APP_THREADWAITNORMAL_SOURCES src/*.c src/*.yasm)

add_executable(ThreadWaitNormal.exe ${APP_THREADWAITNORMAL_SOURCES})
target_include_directories(ThreadWaitNormal.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_THREADWAITNORMAL_INCLUDES}>)
target_compile_options(ThreadWaitNormal.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_THREADWAITNORMAL_PDB_FLAG})
target_compile_definitions(ThreadWaitNormal.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(ThreadWaitNormal.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(ThreadWaitNormal.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS ThreadWaitNormal.exe DESTINATION apps/ThreadWaitNormal)
install(FILES $<TARGET_FILE_DIR:ThreadWaitNormal.exe>/ThreadWaitNormal.pdb DESTINATION apps/ThreadWaitNormal OPTIONAL)
