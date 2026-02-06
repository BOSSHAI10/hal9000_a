cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_FILEREADKERNEL_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_FILEREADKERNEL_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:FileReadKernel.exe>/FileReadKernel.pdb>
    )
endif()

file(GLOB_RECURSE APP_FILEREADKERNEL_SOURCES src/*.c src/*.yasm)

add_executable(FileReadKernel.exe ${APP_FILEREADKERNEL_SOURCES})
target_include_directories(FileReadKernel.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_FILEREADKERNEL_INCLUDES}>)
target_compile_options(FileReadKernel.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_FILEREADKERNEL_PDB_FLAG})
target_compile_definitions(FileReadKernel.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(FileReadKernel.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(FileReadKernel.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS FileReadKernel.exe DESTINATION apps/FileReadKernel)
install(FILES $<TARGET_FILE_DIR:FileReadKernel.exe>/FileReadKernel.pdb DESTINATION apps/FileReadKernel OPTIONAL)
