cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_BADREADKERNEL_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_BADREADKERNEL_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:BadReadKernel.exe>/BadReadKernel.pdb>
    )
endif()

file(GLOB_RECURSE APP_BADREADKERNEL_SOURCES src/*.c src/*.yasm)

add_executable(BadReadKernel.exe ${APP_BADREADKERNEL_SOURCES})
target_include_directories(BadReadKernel.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_BADREADKERNEL_INCLUDES}>)
target_compile_options(BadReadKernel.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_BADREADKERNEL_PDB_FLAG})
target_compile_definitions(BadReadKernel.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(BadReadKernel.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(BadReadKernel.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS BadReadKernel.exe DESTINATION apps/BadReadKernel)
install(FILES $<TARGET_FILE_DIR:BadReadKernel.exe>/BadReadKernel.pdb DESTINATION apps/BadReadKernel OPTIONAL)
