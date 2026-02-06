cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_STACKGROWTHVARIABLES_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_STACKGROWTHVARIABLES_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:StackGrowthVariables.exe>/StackGrowthVariables.pdb>
    )
endif()

file(GLOB_RECURSE APP_STACKGROWTHVARIABLES_SOURCES src/*.c src/*.yasm)

add_executable(StackGrowthVariables.exe ${APP_STACKGROWTHVARIABLES_SOURCES})
target_include_directories(StackGrowthVariables.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_STACKGROWTHVARIABLES_INCLUDES}>)
target_compile_options(StackGrowthVariables.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_STACKGROWTHVARIABLES_PDB_FLAG})
target_compile_definitions(StackGrowthVariables.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(StackGrowthVariables.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(StackGrowthVariables.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS StackGrowthVariables.exe DESTINATION apps/StackGrowthVariables)
install(FILES $<TARGET_FILE_DIR:StackGrowthVariables.exe>/StackGrowthVariables.pdb DESTINATION apps/StackGrowthVariables OPTIONAL)
