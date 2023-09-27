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
        {
            std::lock_guard<std::mutex> guard{lock_};
            i++;
        }
        this->sleep();
    }

    int get_count()
    {
        std::lock_guard<std::mutex> guard{lock_};
        return i;
    }

private:
    int i;
};


TEST_CASE("ThreadManagerBase.start", "[ThreadManagerBase]")
{
    auto uut_ = MockManager{500};
    REQUIRE(uut_.state() == common::ManagedState::Uninitialized);
    uut_.start();
    REQUIRE(uut_.state() == common::ManagedState::Running);
    std::this_thread::sleep_for(std::chrono::microseconds(1000));
    REQUIRE(uut_.get_count() > 0);
    uut_.stop();
    
}

TEST_CASE("ThreadManagerBase.stop", "[ThreadManagerBase]")
{
    auto uut_ = MockManager{500};
    uut_.start();
    uut_.stop();
    auto res = uut_.get_count();
    REQUIRE(uut_.state() == common::ManagedState::Terminated);

    std::this_thread::sleep_for (std::chrono::microseconds(1000));
    REQUIRE(res == uut_.get_count());
}

TEST_CASE("ThreadManagerBase.pause", "[ThreadManagerBase]")
{
    auto uut_ = MockManager{500};
    uut_.start();
    uut_.pause();
    auto res = uut_.get_count();
    REQUIRE(uut_.state() == common::ManagedState::Suspended);

    std::this_thread::sleep_for (std::chrono::microseconds(1000));
    REQUIRE(res == uut_.get_count());
    uut_.stop();

}

TEST_CASE("ThreadManagerBase.resume", "[ThreadManagerBase]")
{
    auto uut_ = MockManager{500};
    uut_.start();
    std::this_thread::sleep_for (std::chrono::microseconds(1000));
    
    uut_.pause();
    auto res = uut_.get_count();
    uut_.resume();
    REQUIRE(uut_.state() == common::ManagedState::Running);

    std::this_thread::sleep_for (std::chrono::microseconds(1000));
    REQUIRE(res < uut_.get_count());
    uut_.stop();
}

TEST_CASE("ThreadManagerBase.sleep_duration", "[ThreadManagerBase]")
{
    auto uut_ = MockManager{500};
    REQUIRE(uut_.sleep_duration() == std::chrono::microseconds(500));

    //set to new microsecond value
    auto set_micro = std::chrono::microseconds(500);
    uut_.sleep_duration(set_micro);
    REQUIRE(uut_.sleep_duration() == set_micro);
    
    //set to new int value
    int set_int = 5000;
    uut_.sleep_duration(set_int);
    REQUIRE(uut_.sleep_duration() == std::chrono::microseconds(set_int));
    uut_.stop();
}