cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(PE_PARSER_INCLUDES
    ${CMAKE_SOURCE_DIR}/CompilerAbstraction/inc
    ${CMAKE_CURRENT_LIST_DIR}/headers
    ${PROJECT_SOURCE_DIR}/CommonLib/inc
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(PE_PARSER_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:PE_Parser>/PE_Parser.pdb>
    )
endif()

file(GLOB_RECURSE PE_PARSER_SOURCES src/*.c src/*.yasm)

add_library(PE_Parser STATIC ${PE_PARSER_SOURCES})
target_include_directories(PE_Parser PRIVATE $<$<COMPILE_LANGUAGE:C>:${PE_PARSER_INCLUDES}> PUBLIC $<$<COMPILE_LANGUAGE:C>:${CMAKE_CURRENT_LIST_DIR}/inc>)
target_compile_options(PE_Parser PRIVATE ${GLOBAL_FLAGS} PRIVATE ${GL_RELEASE_FLAG} PRIVATE ${PE_PARSER_PDB_FLAG})
target_compile_definitions(PE_Parser PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})

install(TARGETS PE_Parser DESTINATION lib)
install(FILES $<TARGET_FILE_DIR:PE_Parser>/PE_Parser.pdb DESTINATION lib OPTIONAL)
