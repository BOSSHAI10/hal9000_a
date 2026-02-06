cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_LIGHTPROJECTAPP_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_LIGHTPROJECTAPP_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:LightProjectApp.exe>/LightProjectApp.pdb>
    )
endif()

file(GLOB_RECURSE APP_LIGHTPROJECTAPP_SOURCES src/*.c src/*.yasm)

add_executable(LightProjectApp.exe ${APP_LIGHTPROJECTAPP_SOURCES})
target_include_directories(LightProjectApp.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_LIGHTPROJECTAPP_INCLUDES}>)
target_compile_options(LightProjectApp.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_LIGHTPROJECTAPP_PDB_FLAG})
target_compile_definitions(LightProjectApp.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(LightProjectApp.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(LightProjectApp.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS LightProjectApp.exe DESTINATION apps/LightProjectApp)
install(FILES $<TARGET_FILE_DIR:LightProjectApp.exe>/LightProjectApp.pdb DESTINATION apps/LightProjectApp OPTIONAL)
