cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_VIRTUALALLOCHUGELAZY_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_VIRTUALALLOCHUGELAZY_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:VirtualAllocHugeLazy.exe>/VirtualAllocHugeLazy.pdb>
    )
endif()

file(GLOB_RECURSE APP_VIRTUALALLOCHUGELAZY_SOURCES src/*.c src/*.yasm)

add_executable(VirtualAllocHugeLazy.exe ${APP_VIRTUALALLOCHUGELAZY_SOURCES})
target_include_directories(VirtualAllocHugeLazy.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_VIRTUALALLOCHUGELAZY_INCLUDES}>)
target_compile_options(VirtualAllocHugeLazy.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_VIRTUALALLOCHUGELAZY_PDB_FLAG})
target_compile_definitions(VirtualAllocHugeLazy.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(VirtualAllocHugeLazy.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(VirtualAllocHugeLazy.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS VirtualAllocHugeLazy.exe DESTINATION apps/VirtualAllocHugeLazy)
install(FILES $<TARGET_FILE_DIR:VirtualAllocHugeLazy.exe>/VirtualAllocHugeLazy.pdb DESTINATION apps/VirtualAllocHugeLazy OPTIONAL)
