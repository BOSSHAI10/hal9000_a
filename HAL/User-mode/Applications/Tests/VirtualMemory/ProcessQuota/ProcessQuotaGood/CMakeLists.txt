cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(APP_PROCESSQUOTAGOOD_INCLUDES
    ${PROJECT_SOURCE_DIR}/Shared/common
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(APP_PROCESSQUOTAGOOD_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:ProcessQuotaGood.exe>/ProcessQuotaGood.pdb>
    )
endif()

file(GLOB_RECURSE APP_PROCESSQUOTAGOOD_SOURCES src/*.c src/*.yasm)

add_executable(ProcessQuotaGood.exe ${APP_PROCESSQUOTAGOOD_SOURCES})
target_include_directories(ProcessQuotaGood.exe PUBLIC $<$<COMPILE_LANGUAGE:C>:${APP_PROCESSQUOTAGOOD_INCLUDES}>)
target_compile_options(ProcessQuotaGood.exe PRIVATE ${APP_FLAGS} PRIVATE ${APP_PROCESSQUOTAGOOD_PDB_FLAG})
target_compile_definitions(ProcessQuotaGood.exe PRIVATE ${GLOBAL_DEFINES} PRIVATE ${MBCS_DEFINE})
target_link_libraries(ProcessQuotaGood.exe PRIVATE UsermodeLibrary CommonLibNoLocks CompilerAbstraction)
target_link_options(ProcessQuotaGood.exe PRIVATE ${APP_LINK_FLAGS})

install(TARGETS ProcessQuotaGood.exe DESTINATION apps/ProcessQuotaGood)
install(FILES $<TARGET_FILE_DIR:ProcessQuotaGood.exe>/ProcessQuotaGood.pdb DESTINATION apps/ProcessQuotaGood OPTIONAL)
