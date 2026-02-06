cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_BADJUMPKERNEL_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_BADJUMPKERNEL_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:BadJumpKernel.exe>/BadJumpKernel.pdb>
    )
endif()

file(GLOB_RECURSE APP_BADJUMPKERNEL_SOURCES src/*.c src/*.yasm)

add_executable(BadJumpKernel.exe ${APP_BADJUMPKERNEL_SOURCES})
target_include_directories(BadJumpKernel.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_BADJUMPKERNEL_INCLUDES}>)
target_compile_options(BadJumpKernel.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_BADJUMPKERNEL_PDB_FLAG})
target_compile_definitions(BadJumpKernel.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(BadJumpKernel.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(BadJumpKernel.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS BadJumpKernel.exe DESTINATION apps/BadJumpKernel)
install(FILES $<TARGET_FILE_DIR:BadJumpKernel.exe>/BadJumpKernel.pdb DESTINATION apps/BadJumpKernel OPTIONAL)
