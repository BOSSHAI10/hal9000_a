cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_VIRTUALFREEINVALID_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_VIRTUALFREEINVALID_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:VirtualFreeInvalid.exe>/VirtualFreeInvalid.pdb>
    )
endif()

file(GLOB_RECURSE APP_VIRTUALFREEINVALID_SOURCES src/*.c src/*.yasm)

add_executable(VirtualFreeInvalid.exe ${APP_VIRTUALFREEINVALID_SOURCES})
target_include_directories(VirtualFreeInvalid.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_VIRTUALFREEINVALID_INCLUDES}>)
target_compile_options(VirtualFreeInvalid.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_VIRTUALFREEINVALID_PDB_FLAG})
target_compile_definitions(VirtualFreeInvalid.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(VirtualFreeInvalid.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(VirtualFreeInvalid.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS VirtualFreeInvalid.exe DESTINATION apps/VirtualFreeInvalid)
install(FILES $<TARGET_FILE_DIR:VirtualFreeInvalid.exe>/VirtualFreeInvalid.pdb DESTINATION apps/VirtualFreeInvalid OPTIONAL)
