#ifndef COMMON_THREAD_MANAGER_HPP_
#define COMMON_THREAD_MANAGER_HPP_

#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <memory>
#include "IManager.hpp"
namespace common
{
    class ThreadManagerBase : public IManager 
    {
    public:
        /**
         * @brief Construct a new Thread Manager Base object
         */
        ThreadManagerBase();

        /**
         * @brief Construct a new Thread Manager Base object
         * @param sleep_us sleep time in microseconds
         */
        explicit ThreadManagerBase(int sleep_us);

        /**
         * @brief Copy construct a new Thread Manager Base object
         * @param to_copy Object to copy from
         * @note explicitly delete copy ctor - not wise to copy threads
         */
        ThreadManagerBase(const ThreadManagerBase& to_copy) = delete;

        /**
         * @brief Move construct a new Thread Manager Base object
         * @param to_move Object to move from
         */
        ThreadManagerBase(ThreadManagerBase&& to_move);

        /**
         * @brief Copy assignment operator
         * 
         * @param to_copy_assign object to copy assign from
         * @return ThreadManagerBase& result (LHS) of assignment
         * @note explicitly delete copy assignment operator - not wise to copy threads
         */
        ThreadManagerBase& operator=(const ThreadManagerBase& to_copy_assign) = delete;
        
        /**
         * @brief Move assignment operator
         * 
         * @param to_move_assign object to move assign from
         * @return ThreadManagerBase& result (LHS) of assignment
         */
        ThreadManagerBase& operator=(ThreadManagerBase&& to_move_assign);

        /**
         * @brief interface method to start the managed feature.
         */
        virtual void start();

        /**
         * @brief interface method to stop the managed feature.
         */
        virtual void stop();

        /**
         * @brief interface method to pause the managed feature.
         */
        virtual void pause();

        /**
         * @brief interface method to resume the managed feature.
         */
        virtual void resume();

        /**
         * @brief interface method returns the current managed state
         * @return ManagedState state of managed thread
         */
        ManagedState state();

        /**
         * @brief Pure virtual thread execution method
         */
        virtual void execute() = 0;
    
    protected:
        std::mutex lock_; //lock for entire manager
        ManagedState state_; //state value for manager
        std::thread process_; //execution thread for managed content
        std::chrono::microseconds sleep_duration_; //duration of a sleep cycle
    
    private:
        std::condition_variable interrupt_signal_; //interrupt variable to wake from sleep
        std::mutex sleep_lock_; //mutex specifically for sleep interruption
    };
}

#endif //COMMON_THREAD_MANAGER_HPP_