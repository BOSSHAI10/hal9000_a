cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_DUMMY_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_DUMMY_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:Dummy.exe>/Dummy.pdb>
    )
endif()

file(GLOB_RECURSE APP_DUMMY_SOURCES src/*.c src/*.yasm)

add_executable(Dummy.exe ${APP_DUMMY_SOURCES})
target_include_directories(Dummy.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_DUMMY_INCLUDES}>)
target_compile_options(Dummy.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_DUMMY_PDB_FLAG})
target_compile_definitions(Dummy.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(Dummy.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(Dummy.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS Dummy.exe DESTINATION apps/Dummy)
install(FILES $<TARGET_FILE_DIR:Dummy.exe>/Dummy.pdb DESTINATION apps/Dummy OPTIONAL)
