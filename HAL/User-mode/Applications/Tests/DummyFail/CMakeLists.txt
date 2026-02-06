cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_DUMMYFAIL_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_DUMMYFAIL_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:DummyFail.exe>/DummyFail.pdb>
    )
endif()

file(GLOB_RECURSE APP_DUMMYFAIL_SOURCES src/*.c src/*.yasm)

add_executable(DummyFail.exe ${APP_DUMMYFAIL_SOURCES})
target_include_directories(DummyFail.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_DUMMYFAIL_INCLUDES}>)
target_compile_options(DummyFail.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_DUMMYFAIL_PDB_FLAG})
target_compile_definitions(DummyFail.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(DummyFail.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(DummyFail.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS DummyFail.exe DESTINATION apps/DummyFail)
install(FILES $<TARGET_FILE_DIR:DummyFail.exe>/DummyFail.pdb DESTINATION apps/DummyFail OPTIONAL)
