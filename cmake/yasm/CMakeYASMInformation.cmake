if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows" AND NOT FORCE_ELF)
    set(CMAKE_YASM_COMPILE_OBJECT 
        "<CMAKE_YASM_COMPILER> <DEFINES> <INCLUDES> -Xvc -pnasm -rnasm -f win64 -o <OBJECT> --objext=yasm.obj <SOURCE>"
    )
else()
    if (UEFI_BUILD)
        set(NASM_BIN_FORMAT "-f win64")
    else()
        set(NASM_BIN_FORMAT "-f elf64 -gdwarf")
    endif()

    set(CMAKE_YASM_COMPILE_OBJECT 
        "<CMAKE_YASM_COMPILER> <DEFINES> <INCLUDES> ${NASM_BIN_FORMAT} -w-label-orphan -w-number-overflow -o <OBJECT> <SOURCE>"
    )
endif()

set(CMAKE_INCLUDE_FLAG_YASM "-I")

set(CMAKE_YASM_INFORMATION_LOADED 1)
