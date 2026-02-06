cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_PROCESSQUOTAJUSTRIGHT_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_PROCESSQUOTAJUSTRIGHT_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:ProcessQuotaJustRight.exe>/ProcessQuotaJustRight.pdb>
    )
endif()

file(GLOB_RECURSE APP_PROCESSQUOTAJUSTRIGHT_SOURCES src/*.c src/*.yasm)

add_executable(ProcessQuotaJustRight.exe ${APP_PROCESSQUOTAJUSTRIGHT_SOURCES})
target_include_directories(ProcessQuotaJustRight.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_PROCESSQUOTAJUSTRIGHT_INCLUDES}>)
target_compile_options(ProcessQuotaJustRight.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_PROCESSQUOTAJUSTRIGHT_PDB_FLAG})
target_compile_definitions(ProcessQuotaJustRight.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(ProcessQuotaJustRight.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(ProcessQuotaJustRight.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS ProcessQuotaJustRight.exe DESTINATION apps/ProcessQuotaJustRight)
install(FILES $<TARGET_FILE_DIR:ProcessQuotaJustRight.exe>/ProcessQuotaJustRight.pdb DESTINATION apps/ProcessQuotaJustRight OPTIONAL)
