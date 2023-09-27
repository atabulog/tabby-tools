add_library(CommonLib
    ThreadManagerBase.cpp)

target_include_directories(CommonLib PUBLIC 
    "${PROJECT_SOURCE_DIR}/common/include")