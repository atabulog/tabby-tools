#include "ThreadManagerBase.hpp"

using namespace common;

ThreadManagerBase::ThreadManagerBase():
    state_{ManagedState::Uninitialized},
    sleep_duration_{std::chrono::microseconds(1000)},
    lock_{},
    interrupt_signal_{},
    sleep_lock_{}
{}

ThreadManagerBase::ThreadManagerBase(int sleep_us):
    state_{ManagedState::Uninitialized},
    sleep_duration_{std::chrono::microseconds(sleep_us)},
    lock_{},
    interrupt_signal_{},
    sleep_lock_{}
{}

ThreadManagerBase::ThreadManagerBase(ThreadManagerBase&& to_move):
    state_{std::move(to_move.state_)},
    sleep_duration_{std::move(to_move.sleep_duration_)},
    lock_{},
    interrupt_signal_{},
    sleep_lock_{}
{}

ThreadManagerBase& ThreadManagerBase::operator=(ThreadManagerBase&& to_move_assign)
{
    state_ = std::move(to_move_assign.state_);
    sleep_duration_ = std::move(to_move_assign.sleep_duration_);
    return *this;
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
        process_.join();
    }
}

void ThreadManagerBase::pause()
{
    if(state_ == ManagedState::Running)
    {
        state_ = ManagedState::Suspended;
    }
}

void ThreadManagerBase::resume()
{
    if(state_ == ManagedState::Suspended)
    {
        state_ = ManagedState::Running;
    }
}

ManagedState ThreadManagerBase::state()
{
    return state_;
}

