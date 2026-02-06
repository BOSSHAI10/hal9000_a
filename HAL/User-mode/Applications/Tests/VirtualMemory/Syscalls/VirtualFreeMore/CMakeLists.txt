cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_VIRTUALFREEMORE_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_VIRTUALFREEMORE_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:VirtualFreeMore.exe>/VirtualFreeMore.pdb>
    )
endif()

file(GLOB_RECURSE APP_VIRTUALFREEMORE_SOURCES src/*.c src/*.yasm)

add_executable(VirtualFreeMore.exe ${APP_VIRTUALFREEMORE_SOURCES})
target_include_directories(VirtualFreeMore.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_VIRTUALFREEMORE_INCLUDES}>)
target_compile_options(VirtualFreeMore.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_VIRTUALFREEMORE_PDB_FLAG})
target_compile_definitions(VirtualFreeMore.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(VirtualFreeMore.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(VirtualFreeMore.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS VirtualFreeMore.exe DESTINATION apps/VirtualFreeMore)
install(FILES $<TARGET_FILE_DIR:VirtualFreeMore.exe>/VirtualFreeMore.pdb DESTINATION apps/VirtualFreeMore OPTIONAL)
