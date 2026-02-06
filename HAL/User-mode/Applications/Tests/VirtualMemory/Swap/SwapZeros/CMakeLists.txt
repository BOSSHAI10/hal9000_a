cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_SWAPZEROS_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_SWAPZEROS_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:SwapZeros.exe>/SwapZeros.pdb>
    )
endif()

file(GLOB_RECURSE APP_SWAPZEROS_SOURCES src/*.c src/*.yasm)

add_executable(SwapZeros.exe ${APP_SWAPZEROS_SOURCES})
target_include_directories(SwapZeros.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_SWAPZEROS_INCLUDES}>)
target_compile_options(SwapZeros.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_SWAPZEROS_PDB_FLAG})
target_compile_definitions(SwapZeros.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(SwapZeros.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(SwapZeros.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS SwapZeros.exe DESTINATION apps/SwapZeros)
install(FILES $<TARGET_FILE_DIR:SwapZeros.exe>/SwapZeros.pdb DESTINATION apps/SwapZeros OPTIONAL)
