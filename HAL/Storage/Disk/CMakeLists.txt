cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(DISK_INCLUDES
    ${CMAKE_SOURCE_DIR}/CompilerAbstraction/inc
    ${HAL9000_HEADERS}
    ${CMAKE_CURRENT_LIST_DIR}/headers
    ${PROJECT_SOURCE_DIR}/CommonLib/inc
    ${PROJECT_SOURCE_DIR}/Shared/common
    ${PROJECT_SOURCE_DIR}/Shared/kernel
    ${PROJECT_SOURCE_DIR}/HAL/inc
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(DISK_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:Disk>/Disk.pdb>
    )
endif()

file(GLOB_RECURSE DISK_SOURCES src/*.c src/*.yasm)

add_library(Disk STATIC ${DISK_SOURCES})
target_include_directories(Disk PRIVATE $<$<COMPILE_LANGUAGE:C>:${DISK_INCLUDES}> PUBLIC $<$<COMPILE_LANGUAGE:C>:${CMAKE_CURRENT_LIST_DIR}/inc>)
target_compile_options(Disk PRIVATE ${GLOBAL_FLAGS} PRIVATE ${GL_RELEASE_FLAG} PRIVATE ${DISK_PDB_FLAG})
target_compile_definitions(Disk PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})

install(TARGETS Disk DESTINATION lib)
install(FILES $<TARGET_FILE_DIR:Disk>/Disk.pdb DESTINATION lib OPTIONAL)
