cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_BADWRITENONCANONICAL_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_BADWRITENONCANONICAL_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:BadWriteNonCanonical.exe>/BadWriteNonCanonical.pdb>
    )
endif()

file(GLOB_RECURSE APP_BADWRITENONCANONICAL_SOURCES src/*.c src/*.yasm)

add_executable(BadWriteNonCanonical.exe ${APP_BADWRITENONCANONICAL_SOURCES})
target_include_directories(BadWriteNonCanonical.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_BADWRITENONCANONICAL_INCLUDES}>)
target_compile_options(BadWriteNonCanonical.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_BADWRITENONCANONICAL_PDB_FLAG})
target_compile_definitions(BadWriteNonCanonical.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(BadWriteNonCanonical.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(BadWriteNonCanonical.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS BadWriteNonCanonical.exe DESTINATION apps/BadWriteNonCanonical)
install(FILES $<TARGET_FILE_DIR:BadWriteNonCanonical.exe>/BadWriteNonCanonical.pdb DESTINATION apps/BadWriteNonCanonical OPTIONAL)
