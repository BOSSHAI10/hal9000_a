cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(USERMODE_LIBRARY_INCLUDES
    ${CMAKE_CURRENT_LIST_DIR}/headers
    ${PROJECT_SOURCE_DIR}/CommonLib/inc
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(USERMODE_LIBRARY_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:UsermodeLibrary>/UsermodeLibrary.pdb>
    )
endif()

file(GLOB_RECURSE USERMODE_LIBRARY_SOURCES src/*.c src/*.yasm)

add_library(UsermodeLibrary STATIC ${USERMODE_LIBRARY_SOURCES})
target_include_directories(UsermodeLibrary PRIVATE $<$<COMPILE_LANGUAGE:C>:${USERMODE_LIBRARY_INCLUDES}> PUBLIC $<$<COMPILE_LANGUAGE:C>:${CMAKE_CURRENT_LIST_DIR}/inc> PUBLIC ${CMAKE_SOURCE_DIR}/CompilerAbstraction/inc)
target_compile_options(UsermodeLibrary PRIVATE ${GLOBAL_FLAGS} PRIVATE ${GL_RELEASE_FLAG} PRIVATE ${USERMODE_LIBRARY_PDB_FLAG})
target_compile_definitions(UsermodeLibrary PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})

install(TARGETS UsermodeLibrary DESTINATION lib)
install(FILES $<TARGET_FILE_DIR:UsermodeLibrary>/UsermodeLibrary.pdb DESTINATION lib OPTIONAL)
