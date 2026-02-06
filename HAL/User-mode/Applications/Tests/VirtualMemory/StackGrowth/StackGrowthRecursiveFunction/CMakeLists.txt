cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_STACKGROWTHRECURSIVEFUNCTION_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_STACKGROWTHRECURSIVEFUNCTION_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:StackGrowthRecursiveFunction.exe>/StackGrowthRecursiveFunction.pdb>
    )
endif()

file(GLOB_RECURSE APP_STACKGROWTHRECURSIVEFUNCTION_SOURCES src/*.c src/*.yasm)

add_executable(StackGrowthRecursiveFunction.exe ${APP_STACKGROWTHRECURSIVEFUNCTION_SOURCES})
target_include_directories(StackGrowthRecursiveFunction.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_STACKGROWTHRECURSIVEFUNCTION_INCLUDES}>)
target_compile_options(StackGrowthRecursiveFunction.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_STACKGROWTHRECURSIVEFUNCTION_PDB_FLAG})
target_compile_definitions(StackGrowthRecursiveFunction.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(StackGrowthRecursiveFunction.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(StackGrowthRecursiveFunction.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS StackGrowthRecursiveFunction.exe DESTINATION apps/StackGrowthRecursiveFunction)
install(FILES $<TARGET_FILE_DIR:StackGrowthRecursiveFunction.exe>/StackGrowthRecursiveFunction.pdb DESTINATION apps/StackGrowthRecursiveFunction OPTIONAL)
