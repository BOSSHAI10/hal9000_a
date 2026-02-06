cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(HAL_INCLUDES 
    ${CMAKE_SOURCE_DIR}/CompilerAbstraction/inc
    ${CMAKE_CURRENT_LIST_DIR}/headers
    ${PROJECT_SOURCE_DIR}/CommonLib/inc
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(HAL_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:HAL>/HAL.pdb>
    )
endif()

file(GLOB_RECURSE HAL_SOURCES src/*.c src/*.yasm)

add_library(HAL STATIC ${HAL_SOURCES})
target_include_directories(HAL PRIVATE $<$<COMPILE_LANGUAGE:C>:${HAL_INCLUDES}> PUBLIC $<$<COMPILE_LANGUAGE:C>:${CMAKE_CURRENT_LIST_DIR}/inc>)
target_compile_options(HAL PRIVATE ${GLOBAL_FLAGS} PRIVATE ${HAL_PDB_FLAG})
target_compile_definitions(HAL PRIVATE ${GLOBAL_DEFINES} PRIVATE ${UNICODE_DEFINES})

install(TARGETS HAL DESTINATION lib)
install(FILES $<TARGET_FILE_DIR:HAL>/HAL.pdb DESTINATION lib OPTIONAL)
