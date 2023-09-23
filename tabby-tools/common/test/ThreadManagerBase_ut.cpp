#include <memory>
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

    MockManager(int sleep_us):
        common::ThreadManagerBase(sleep_us),
        i{0}
    {}

    void execute()
    {
        i++;
        sleep();
    }

private:
    int i;
};

class ManagerTestFixture
{
public:
    ManagerTestFixture()
    {
        duration_ = std::chrono::microseconds(10000);
        uut_ = MockManager{(int) duration_.count()};
    }

protected:
    MockManager uut_;

private:
    std::chrono::microseconds duration_;
};

TEST_CASE_METHOD(ManagerTestFixture, "verify start method", "[ThreadManagerBase]")
{
    REQUIRE(uut_.state() == common::ManagedState::Uninitialized);
    uut_.start();
    REQUIRE(uut_.state() == common::ManagedState::Running);
}

TEST_CASE_METHOD(ManagerTestFixture, "verify stop method", "[ThreadManagerBase]")
{
    uut_.start();
    uut_.stop();
    REQUIRE(uut_.state() == common::ManagedState::Terminated);
}

TEST_CASE_METHOD(ManagerTestFixture, "verify pause method", "[ThreadManagerBase]")
{
    uut_.start();
    uut_.pause();
    REQUIRE(uut_.state() == common::ManagedState::Suspended);
}

TEST_CASE_METHOD(ManagerTestFixture, "verify resume method", "[ThreadManagerBase]")
{
    uut_.start();
    uut_.pause();
    uut_.resume();
    REQUIRE(uut_.state() == common::ManagedState::Running);
}