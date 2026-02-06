cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_APPHELLOWORLD_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_APPHELLOWORLD_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:AppHelloWorld.exe>/AppHelloWorld.pdb>
    )
endif()

file(GLOB_RECURSE APP_APPHELLOWORLD_SOURCES src/*.c src/*.yasm)

add_executable(AppHelloWorld.exe ${APP_APPHELLOWORLD_SOURCES})
target_include_directories(AppHelloWorld.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_APPHELLOWORLD_INCLUDES}>)
target_compile_options(AppHelloWorld.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_APPHELLOWORLD_PDB_FLAG})
target_compile_definitions(AppHelloWorld.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(AppHelloWorld.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(AppHelloWorld.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS AppHelloWorld.exe DESTINATION apps/AppHelloWorld)
install(FILES $<TARGET_FILE_DIR:AppHelloWorld.exe>/AppHelloWorld.pdb DESTINATION apps/AppHelloWorld OPTIONAL)
