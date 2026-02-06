cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_BADWRITEIOPORT_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_BADWRITEIOPORT_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:BadWriteIoPort.exe>/BadWriteIoPort.pdb>
    )
endif()

file(GLOB_RECURSE APP_BADWRITEIOPORT_SOURCES src/*.c src/*.yasm)

add_executable(BadWriteIoPort.exe ${APP_BADWRITEIOPORT_SOURCES})
target_include_directories(BadWriteIoPort.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_BADWRITEIOPORT_INCLUDES}>)
target_compile_options(BadWriteIoPort.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_BADWRITEIOPORT_PDB_FLAG})
target_compile_definitions(BadWriteIoPort.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(BadWriteIoPort.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(BadWriteIoPort.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS BadWriteIoPort.exe DESTINATION apps/BadWriteIoPort)
install(FILES $<TARGET_FILE_DIR:BadWriteIoPort.exe>/BadWriteIoPort.pdb DESTINATION apps/BadWriteIoPort OPTIONAL)
