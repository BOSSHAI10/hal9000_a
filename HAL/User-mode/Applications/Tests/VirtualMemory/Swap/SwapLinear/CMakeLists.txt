cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_SWAPLINEAR_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_SWAPLINEAR_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:SwapLinear.exe>/SwapLinear.pdb>
    )
endif()

file(GLOB_RECURSE APP_SWAPLINEAR_SOURCES src/*.c src/*.yasm)

add_executable(SwapLinear.exe ${APP_SWAPLINEAR_SOURCES})
target_include_directories(SwapLinear.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_SWAPLINEAR_INCLUDES}>)
target_compile_options(SwapLinear.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_SWAPLINEAR_PDB_FLAG})
target_compile_definitions(SwapLinear.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(SwapLinear.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(SwapLinear.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS SwapLinear.exe DESTINATION apps/SwapLinear)
install(FILES $<TARGET_FILE_DIR:SwapLinear.exe>/SwapLinear.pdb DESTINATION apps/SwapLinear OPTIONAL)
