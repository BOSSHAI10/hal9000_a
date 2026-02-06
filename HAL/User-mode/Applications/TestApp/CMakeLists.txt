cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_TESTAPP_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_TESTAPP_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:TestApp.exe>/TestApp.pdb>
    )
endif()

file(GLOB_RECURSE APP_TESTAPP_SOURCES src/*.c src/*.yasm)

add_executable(TestApp.exe ${APP_TESTAPP_SOURCES})
target_include_directories(TestApp.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_TESTAPP_INCLUDES}>)
target_compile_options(TestApp.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_TESTAPP_PDB_FLAG})
target_compile_definitions(TestApp.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(TestApp.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(TestApp.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS TestApp.exe DESTINATION apps/TestApp)
install(FILES $<TARGET_FILE_DIR:TestApp.exe>/TestApp.pdb DESTINATION apps/TestApp OPTIONAL)
