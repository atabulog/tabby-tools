#include <chrono>
#include <memory>
#include <thread>
#include <iostream>
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
        this->sleep();
    }

    int get_count()
    {
        std::cout << "yeet" << std::endl;
        return i;
    }

private:
    int i;
};

class ManagerTestFixture
{
public:
    ManagerTestFixture()
    {
        duration_ = std::chrono::microseconds(50);
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
    
    std::this_thread::sleep_for (std::chrono::microseconds(1000));
    REQUIRE(uut_.get_count() > 0);
    
}

TEST_CASE_METHOD(ManagerTestFixture, "verify stop method", "[ThreadManagerBase]")
{
    uut_.start();
    uut_.stop();
    auto res = uut_.get_count();
    REQUIRE(uut_.state() == common::ManagedState::Terminated);

    std::this_thread::sleep_for (std::chrono::microseconds(1000));
    REQUIRE(res == uut_.get_count());
}

TEST_CASE_METHOD(ManagerTestFixture, "verify pause method", "[ThreadManagerBase]")
{
    uut_.start();
    uut_.pause();
    auto res = uut_.get_count();
    REQUIRE(uut_.state() == common::ManagedState::Suspended);

    std::this_thread::sleep_for (std::chrono::microseconds(1000));
    REQUIRE(res == uut_.get_count());

}

TEST_CASE_METHOD(ManagerTestFixture, "verify resume method", "[ThreadManagerBase]")
{
    uut_.start();
    std::this_thread::sleep_for (std::chrono::microseconds(1000));
    
    uut_.pause();
    auto res = uut_.get_count();
    uut_.resume();
    REQUIRE(uut_.state() == common::ManagedState::Running);

    std::this_thread::sleep_for (std::chrono::microseconds(1000));
    REQUIRE(res < uut_.get_count());
}

TEST_CASE_METHOD(ManagerTestFixture, "verify sleep duration accessors", "[ThreadManagerBase]")
{
    REQUIRE(uut_.sleep_duration() == std::chrono::microseconds(50));

    //set to new microsecond value
    auto set_micro = std::chrono::microseconds(500);
    uut_.sleep_duration(set_micro);
    REQUIRE(uut_.sleep_duration() == set_micro);
    
    //set to new int value
    int set_int = 5000;
    uut_.sleep_duration(set_int);
    REQUIRE(uut_.sleep_duration() == std::chrono::microseconds(set_int));

}