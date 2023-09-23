
find_package(Catch2 3 REQUIRED)

set(TestCommon ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(CommonTests
    ThreadManagerBase_ut.cpp)

target_link_libraries(CommonTests PUBLIC 
    CommonLib
    Catch2::Catch2WithMain)

include(CTest)
include(ParseAndAddCatchTests)
ParseAndAddCatchTests(CommonTests)

add_test(NAME ${TestCommon} COMMAND ${TestCommon})