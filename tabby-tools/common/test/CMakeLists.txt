
find_package(Catch2 3 REQUIRED)

set(TestCommon ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(CommonTests
    TestMain.cpp
    ThreadManagerBase_ut.cpp
    Suspendable_ut.cpp)

target_link_libraries(CommonTests PUBLIC 
    CommonLib
    Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(CommonTests)

add_test(NAME ${TestCommon} COMMAND ${TestCommon})