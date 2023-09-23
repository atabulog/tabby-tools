#include <catch2/catch_test_macros.hpp>
#include "ThreadManagerBase.hpp"

#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do once

class MockManager: public common::ThreadManagerBase
{
public:
    MockManager():
        common::ThreadManagerBase(),
        i{0}
    {}

    void execute()
    {
        i++;
    }
private:
    int i;
};

TEST_CASE("Verify dummy", "ThreadManagerBase")
{
    auto uut = MockManager();

    REQUIRE(uut.sleep_duration() == std::chrono::microseconds(1000));
}