cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(ELF_PARSER_INCLUDES
    ${CMAKE_SOURCE_DIR}/CompilerAbstraction/inc
    ${CMAKE_CURRENT_LIST_DIR}/headers
    ${PROJECT_SOURCE_DIR}/CommonLib/inc
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(ELF_PARSER_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:ELF_Parser>/ELF_Parser.pdb>
    )
endif()

file(GLOB_RECURSE ELF_PARSER_SOURCES src/*.c src/*.yasm)

add_library(ELF_Parser STATIC ${ELF_PARSER_SOURCES})
target_include_directories(ELF_Parser PRIVATE $<$<COMPILE_LANGUAGE:C>:${ELF_PARSER_INCLUDES}> PUBLIC $<$<COMPILE_LANGUAGE:C>:${CMAKE_CURRENT_LIST_DIR}/inc>)
target_compile_options(ELF_Parser PRIVATE ${GLOBAL_FLAGS} PRIVATE ${GL_RELEASE_FLAG} PRIVATE ${ELF_PARSER_PDB_FLAG})
target_compile_definitions(ELF_Parser PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})

install(TARGETS ELF_Parser DESTINATION lib)
install(FILES $<TARGET_FILE_DIR:ELF_Parser>/ELF_Parser.pdb DESTINATION lib OPTIONAL)
