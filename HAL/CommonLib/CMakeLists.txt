cmake_minimum_required(VERSION 3.28.3 FATAL_ERROR)

set(COMMON_LIB_INCLUDES
    ${CMAKE_SOURCE_DIR}/CompilerAbstraction/inc
    ${CMAKE_CURRENT_LIST_DIR}/headers 
)

set(COMMON_LIB_DEFINES
    COMMON_LIB_IMPL 
    _CONSOLE
)

set(COMMON_LIB_NON_NATIVE_DEFINES 
    ${COMMON_LIB_DEFINES} 
    CL_NO_RUNTIME_CHECKS
)

set(COMMON_LIB_NO_LOCKS_DEFINES 
    ${COMMON_LIB_DEFINES} 
    _COMMONLIB_NO_LOCKS_
)

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(COMMON_LIB_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:CommonLib>/CommonLib.pdb>
    )

    set(COMMON_LIB_NON_NATIVE_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:CommonLibNonNative>/CommonLibNonNative.pdb>
    )

    set(COMMON_LIB_NO_LOCKS_PDB_FLAG
        $<$<COMPILE_LANGUAGE:C>:/Fd:$<TARGET_FILE_DIR:CommonLibNoLocks>/CommonLibNoLocks.pdb>
    )
endif()

file(GLOB_RECURSE COMMON_LIB_SOURCES src/*.c src/*.yasm)

add_library(CommonLib STATIC ${COMMON_LIB_SOURCES})
target_include_directories(CommonLib PRIVATE $<$<COMPILE_LANGUAGE:C>:${COMMON_LIB_INCLUDES}> PUBLIC $<$<COMPILE_LANGUAGE:C>:${CMAKE_CURRENT_LIST_DIR}/inc>)
target_compile_options(CommonLib PRIVATE ${GLOBAL_FLAGS} PRIVATE ${COMMON_LIB_PDB_FLAG})
target_compile_definitions(CommonLib 
    PRIVATE ${GLOBAL_DEFINES} 
    PRIVATE $<$<COMPILE_LANGUAGE:C>:${COMMON_LIB_DEFINES}> 
    PRIVATE ${UNICODE_DEFINES}
)

add_library(CommonLibNoLocks STATIC ${COMMON_LIB_SOURCES})
target_include_directories(CommonLibNoLocks PRIVATE $<$<COMPILE_LANGUAGE:C>:${COMMON_LIB_INCLUDES}> PUBLIC $<$<COMPILE_LANGUAGE:C>:${CMAKE_CURRENT_LIST_DIR}/inc>)
target_compile_options(CommonLibNoLocks PRIVATE ${GLOBAL_FLAGS} PRIVATE ${COMMON_LIB_NO_LOCKS_PDB_FLAG})
target_compile_definitions(CommonLibNoLocks 
    PRIVATE ${GLOBAL_DEFINES}
    PRIVATE $<$<COMPILE_LANGUAGE:C>:${COMMON_LIB_NO_LOCKS_DEFINES}>
    PRIVATE ${UNICODE_DEFINES}
)

add_library(CommonLibNonNative STATIC ${COMMON_LIB_SOURCES})
target_include_directories(CommonLibNonNative PRIVATE $<$<COMPILE_LANGUAGE:C>:${COMMON_LIB_INCLUDES}> PUBLIC $<$<COMPILE_LANGUAGE:C>:${CMAKE_CURRENT_LIST_DIR}/inc>)
target_compile_options(CommonLibNonNative PRIVATE ${GLOBAL_FLAGS} PRIVATE ${COMMON_LIB_NON_NATIVE_PDB_FLAG})
target_compile_definitions(CommonLibNonNative 
    PRIVATE ${GLOBAL_DEFINES} 
    PRIVATE $<$<COMPILE_LANGUAGE:C>:${COMMON_LIB_NON_NATIVE_DEFINES}>
    PRIVATE ${UNICODE_DEFINES}
)

install(TARGETS CommonLib DESTINATION lib)
install(FILES $<TARGET_FILE_DIR:CommonLib>/CommonLib.pdb DESTINATION lib OPTIONAL)

install(TARGETS CommonLibNoLocks DESTINATION lib)
install(FILES $<TARGET_FILE_DIR:CommonLibNoLocks>/CommonLibNoLocks.pdb DESTINATION lib OPTIONAL)

install(TARGETS CommonLibNonNative DESTINATION lib)
install(FILES $<TARGET_FILE_DIR:CommonLibNonNative>/CommonLibNonNative.pdb DESTINATION lib OPTIONAL)
