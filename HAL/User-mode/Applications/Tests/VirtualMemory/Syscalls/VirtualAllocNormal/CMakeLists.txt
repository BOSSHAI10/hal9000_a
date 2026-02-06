cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_VIRTUALALLOCNORMAL_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_VIRTUALALLOCNORMAL_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:VirtualAllocNormal.exe>/VirtualAllocNormal.pdb>
    )
endif()

file(GLOB_RECURSE APP_VIRTUALALLOCNORMAL_SOURCES src/*.c src/*.yasm)

add_executable(VirtualAllocNormal.exe ${APP_VIRTUALALLOCNORMAL_SOURCES})
target_include_directories(VirtualAllocNormal.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_VIRTUALALLOCNORMAL_INCLUDES}>)
target_compile_options(VirtualAllocNormal.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_VIRTUALALLOCNORMAL_PDB_FLAG})
target_compile_definitions(VirtualAllocNormal.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(VirtualAllocNormal.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(VirtualAllocNormal.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS VirtualAllocNormal.exe DESTINATION apps/VirtualAllocNormal)
install(FILES $<TARGET_FILE_DIR:VirtualAllocNormal.exe>/VirtualAllocNormal.pdb DESTINATION apps/VirtualAllocNormal OPTIONAL)
