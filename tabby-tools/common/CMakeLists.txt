
add_library(common
    "src/ThreadManagerBase.cpp")

target_include_directories(common PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${CMAKE_CURRENT_LIST_DIR}/include"
    )