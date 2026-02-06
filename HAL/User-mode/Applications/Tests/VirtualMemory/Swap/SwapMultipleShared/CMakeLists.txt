cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_SWAPMULTIPLESHARED_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_SWAPMULTIPLESHARED_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:SwapMultipleShared.exe>/SwapMultipleShared.pdb>
    )
endif()

file(GLOB_RECURSE APP_SWAPMULTIPLESHARED_SOURCES src/*.c src/*.yasm)

add_executable(SwapMultipleShared.exe ${APP_SWAPMULTIPLESHARED_SOURCES})
target_include_directories(SwapMultipleShared.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_SWAPMULTIPLESHARED_INCLUDES}>)
target_compile_options(SwapMultipleShared.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_SWAPMULTIPLESHARED_PDB_FLAG})
target_compile_definitions(SwapMultipleShared.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(SwapMultipleShared.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(SwapMultipleShared.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS SwapMultipleShared.exe DESTINATION apps/SwapMultipleShared)
install(FILES $<TARGET_FILE_DIR:SwapMultipleShared.exe>/SwapMultipleShared.pdb DESTINATION apps/SwapMultipleShared OPTIONAL)
