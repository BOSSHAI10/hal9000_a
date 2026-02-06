cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(COMPILER_ABSTRACTION_INCLUDES 
    ${CMAKE_SOURCE_DIR}/CompilerAbstraction/inc
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(COMPILER_ABSTRACTION_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:CompilerAbstraction>/CompilerAbstraction.pdb>
    )
endif()

file(GLOB_RECURSE COMPILER_ABSTRACTION_SOURCES src/*.c src/*.yasm)

add_library(CompilerAbstraction STATIC ${COMPILER_ABSTRACTION_SOURCES})
target_include_directories(CompilerAbstraction PUBLIC $<$<COMPILE_LANGUAGE:C>:${COMPILER_ABSTRACTION_INCLUDES}>)
target_compile_options(CompilerAbstraction PRIVATE ${GLOBAL_FLAGS} PRIVATE ${COMPILER_ABSTRACTION_PDB_FLAG})
target_compile_definitions(CompilerAbstraction PRIVATE ${GLOBAL_DEFINES})

install(TARGETS CompilerAbstraction DESTINATION lib)
install(FILES $<TARGET_FILE_DIR:CompilerAbstraction>/CompilerAbstraction.pdb DESTINATION lib OPTIONAL)
