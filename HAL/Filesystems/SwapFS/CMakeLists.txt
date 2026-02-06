cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(SWAP_FS_INCLUDES 
    ${CMAKE_SOURCE_DIR}/CompilerAbstraction/inc
    ${HAL9000_HEADERS}
    ${CMAKE_CURRENT_LIST_DIR}/headers
    ${PROJECT_SOURCE_DIR}/CommonLib/inc
    ${PROJECT_SOURCE_DIR}/Shared/common
    ${PROJECT_SOURCE_DIR}/Shared/kernel
    ${PROJECT_SOURCE_DIR}/HAL/inc
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(SWAP_FS_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:SwapFS>/SwapFS.pdb>
    )
endif()

file(GLOB_RECURSE SWAP_FS_SOURCES src/*.c src/*.yasm)

add_library(SwapFS STATIC ${SWAP_FS_SOURCES})
target_include_directories(SwapFS PRIVATE $<$<COMPILE_LANGUAGE:C>:${SWAP_FS_INCLUDES}> PUBLIC $<$<COMPILE_LANGUAGE:C>:${CMAKE_CURRENT_LIST_DIR}/inc>)
target_compile_options(SwapFS PRIVATE ${GLOBAL_FLAGS} PRIVATE ${SWAP_FS_PDB_FLAG} PRIVATE ${GL_RELEASE_FLAG})
target_compile_definitions(SwapFS PRIVATE ${GLOBAL_DEFINES} PRIVATE ${UNICODE_DEFINES})

install(TARGETS SwapFS DESTINATION lib)
install(FILES $<TARGET_FILE_DIR:SwapFS>/SwapFS.pdb DESTINATION lib OPTIONAL)
