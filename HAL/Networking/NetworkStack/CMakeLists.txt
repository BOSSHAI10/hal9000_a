cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(NETWORK_STACK_INCLUDES
    ${CMAKE_SOURCE_DIR}/CompilerAbstraction/inc
    ${HAL9000_HEADERS}
    ${CMAKE_CURRENT_LIST_DIR}/headers
    ${PROJECT_SOURCE_DIR}/CommonLib/inc
    ${PROJECT_SOURCE_DIR}/Shared/common
    ${PROJECT_SOURCE_DIR}/Shared/kernel
    ${PROJECT_SOURCE_DIR}/HAL/inc
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(NETWORK_STACK_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:NetworkStack>/NetworkStack.pdb>
    )
endif()

file(GLOB_RECURSE NETWORK_STACK_SOURCES src/*.c src/*.yasm)

add_library(NetworkStack STATIC ${NETWORK_STACK_SOURCES})
target_include_directories(NetworkStack PRIVATE $<$<COMPILE_LANGUAGE:C>:${NETWORK_STACK_INCLUDES}> PUBLIC $<$<COMPILE_LANGUAGE:C>:${CMAKE_CURRENT_LIST_DIR}/inc>)
target_compile_options(NetworkStack PRIVATE ${GLOBAL_FLAGS} PRIVATE ${GL_RELEASE_FLAG} PRIVATE ${NETWORK_STACK_PDB_FLAG})
target_compile_definitions(NetworkStack PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE}) 

install(TARGETS NetworkStack DESTINATION lib)
install(FILES $<TARGET_FILE_DIR:NetworkStack>/NetworkStack.pdb DESTINATION lib OPTIONAL)
