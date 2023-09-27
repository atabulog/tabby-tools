#include <unordered_map>
#include <string>
#include <chrono>
#include <thread>
#include <catch2/catch_test_macros.hpp>
#include "Suspendable.hpp"

class SimpleMockSuspendable: public common::Suspendable
{
public:
    SimpleMockSuspendable()
    {}

    std::string data(const std::string& key)
    {
        if(!is_suspended())
        {
            auto it = data_.find(key);
            if(it != data_.cend())
            {
                return it->second;
            }
            return "";
        }
        return "";
    }

private:
    const std::unordered_map<std::string, std::string> data_{
        {"Apple", "Orange"},
        {"Hammer", "Nail"},
        {"Salt", "Pepper"}
    };
};

TEST_CASE("Suspendable.suspend", "[common::Suspendable]")
{
    //create a suspendable mock
    auto uut_ = SimpleMockSuspendable();
    //verify starting state is not suspended
    REQUIRE_FALSE(uut_.is_suspended());
    //suspend logic, verify state change, and logic reflects change
    uut_.suspend();
    REQUIRE(uut_.is_suspended());
    REQUIRE(uut_.data("Apple") == "");
}

TEST_CASE("Suspendable.resume", "[common::Suspendable]")
{
    //create a suspendable mock
    auto uut_ = SimpleMockSuspendable();
    //suspend data and ensure its suspended
    uut_.suspend();
    REQUIRE(uut_.is_suspended());
    //resume data, verify state change, and logic reflects change
    uut_.resume();
    REQUIRE_FALSE(uut_.is_suspended());
    REQUIRE(uut_.data("Hammer") == "Nail");
}

TEST_CASE("Suspendable concurrency safety", "[common::Suspendable]")
{
    //create suspendable mock
    auto uut_ = SimpleMockSuspendable();
    //create thread that suspends mock
    std::thread suspension_thread(&SimpleMockSuspendable::suspend, uut_);

    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    //verify the suspension set on separate thread is applied to this thread
    REQUIRE(uut_.is_suspended());

    //teardown thread
    suspension_thread.join();
}