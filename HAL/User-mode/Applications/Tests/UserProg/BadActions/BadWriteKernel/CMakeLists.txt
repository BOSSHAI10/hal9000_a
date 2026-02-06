cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_BADWRITEKERNEL_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_BADWRITEKERNEL_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:BadWriteKernel.exe>/BadWriteKernel.pdb>
    )
endif()

file(GLOB_RECURSE APP_BADWRITEKERNEL_SOURCES src/*.c src/*.yasm)

add_executable(BadWriteKernel.exe ${APP_BADWRITEKERNEL_SOURCES})
target_include_directories(BadWriteKernel.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_BADWRITEKERNEL_INCLUDES}>)
target_compile_options(BadWriteKernel.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_BADWRITEKERNEL_PDB_FLAG})
target_compile_definitions(BadWriteKernel.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(BadWriteKernel.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(BadWriteKernel.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS BadWriteKernel.exe DESTINATION apps/BadWriteKernel)
install(FILES $<TARGET_FILE_DIR:BadWriteKernel.exe>/BadWriteKernel.pdb DESTINATION apps/BadWriteKernel OPTIONAL)
