set(LLVM_HINTS ${CMAKE_CURRENT_LIST_DIR}/../tools/llvm/bin)

if (CMAKE_HOST_APPLE)
    list(APPEND LLVM_HINTS /opt/homebrew/opt/llvm/bin)
    list(APPEND LLVM_HINTS /usr/local/opt/llvm/bin)
endif()
