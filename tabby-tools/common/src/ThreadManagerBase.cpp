#include "ThreadManagerBase.hpp"

using namespace common;

ThreadManagerBase::ThreadManagerBase():
    state_{ManagedState::Uninitialized},
    sleep_duration_{std::chrono::microseconds(1000)},
    interrupt_signal_{}
{}

ThreadManagerBase::ThreadManagerBase(int sleep_us):
    state_{ManagedState::Uninitialized},
    sleep_duration_{std::chrono::microseconds(sleep_us)},
    interrupt_signal_{}
{}

ThreadManagerBase::ThreadManagerBase(ThreadManagerBase&& to_move):
    state_{std::move(to_move.state_)},
    sleep_duration_{std::move(to_move.sleep_duration_)},
    interrupt_signal_{}
{}

ThreadManagerBase& ThreadManagerBase::operator=(ThreadManagerBase&& to_move_assign)
{
    state_ = std::move(to_move_assign.state_);
    sleep_duration_ = std::move(to_move_assign.sleep_duration_);
    return *this;
}

ThreadManagerBase::~ThreadManagerBase()
{
    stop();
}

void ThreadManagerBase::start()
{
    if(state_ == ManagedState::Uninitialized)
    {
        std::lock_guard<std::mutex> guard{lock_};
        state_ = ManagedState::Running;
        process_ = std::thread(&process_, this);
    }
}

void ThreadManagerBase::stop()
{
    if(state_ == ManagedState::Running || 
       state_ == ManagedState::Suspended)
    {
        std::lock_guard<std::mutex> guard{lock_};
        state_ = ManagedState::Terminated;
        interrupt_signal_.notify_one();
        process_.join();
    }
}

void ThreadManagerBase::pause()
{
    if(state_ == ManagedState::Running)
    {
        std::lock_guard<std::mutex> guard{lock_};
        state_ = ManagedState::Suspended;
    }
}

void ThreadManagerBase::resume()
{
    if(state_ == ManagedState::Suspended)
    {
        std::lock_guard<std::mutex> guard{lock_};
        state_ = ManagedState::Running;
    }
}

ManagedState ThreadManagerBase::state()
{
    std::lock_guard<std::mutex> guard{lock_};
    return state_;
}

const std::chrono::microseconds& ThreadManagerBase::sleep_duration()
{
    std::lock_guard<std::mutex> guard{lock_};
    return sleep_duration_;
}

void ThreadManagerBase::sleep_duration(const std::chrono::microseconds& duration)
{
    std::lock_guard<std::mutex> guard{lock_};
    sleep_duration_ = duration;
}

void ThreadManagerBase::sleep_duration(const int duration_us)
{
    std::lock_guard<std::mutex> guard{lock_};
    sleep_duration_ = std::chrono::microseconds(duration_us);
}

void ThreadManagerBase::sleep()
{
    if(state_ == ManagedState::Running)
    {
        std::unique_lock<std::mutex> sleep_guard{sleep_lock_};
        interrupt_signal_.wait_for(sleep_guard, sleep_duration_);
    }
}

void ThreadManagerBase::thread_loop()
{
    //terminate thread loop upon state set to end or fault
    while(state() != ManagedState::Fault || 
          state() != ManagedState::Terminated)
    {
        //only execute thread logic if set to running
        if(state() == ManagedState::Running)
        {
            execute();
        }
    }
}