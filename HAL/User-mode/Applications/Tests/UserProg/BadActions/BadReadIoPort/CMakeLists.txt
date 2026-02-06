cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_BADREADIOPORT_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_BADREADIOPORT_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:BadReadIoPort.exe>/BadReadIoPort.pdb>
    )
endif()

file(GLOB_RECURSE APP_BADREADIOPORT_SOURCES src/*.c src/*.yasm)

add_executable(BadReadIoPort.exe ${APP_BADREADIOPORT_SOURCES})
target_include_directories(BadReadIoPort.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_BADREADIOPORT_INCLUDES}>)
target_compile_options(BadReadIoPort.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_BADREADIOPORT_PDB_FLAG})
target_compile_definitions(BadReadIoPort.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(BadReadIoPort.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(BadReadIoPort.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS BadReadIoPort.exe DESTINATION apps/BadReadIoPort)
install(FILES $<TARGET_FILE_DIR:BadReadIoPort.exe>/BadReadIoPort.pdb DESTINATION apps/BadReadIoPort OPTIONAL)
