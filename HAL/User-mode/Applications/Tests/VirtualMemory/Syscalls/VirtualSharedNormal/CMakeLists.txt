cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_VIRTUALSHAREDNORMAL_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_VIRTUALSHAREDNORMAL_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:VirtualSharedNormal.exe>/VirtualSharedNormal.pdb>
    )
endif()

file(GLOB_RECURSE APP_VIRTUALSHAREDNORMAL_SOURCES src/*.c src/*.yasm)

add_executable(VirtualSharedNormal.exe ${APP_VIRTUALSHAREDNORMAL_SOURCES})
target_include_directories(VirtualSharedNormal.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_VIRTUALSHAREDNORMAL_INCLUDES}>)
target_compile_options(VirtualSharedNormal.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_VIRTUALSHAREDNORMAL_PDB_FLAG})
target_compile_definitions(VirtualSharedNormal.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(VirtualSharedNormal.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(VirtualSharedNormal.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS VirtualSharedNormal.exe DESTINATION apps/VirtualSharedNormal)
install(FILES $<TARGET_FILE_DIR:VirtualSharedNormal.exe>/VirtualSharedNormal.pdb DESTINATION apps/VirtualSharedNormal OPTIONAL)
