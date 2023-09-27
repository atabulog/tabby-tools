#ifndef COMMON_SUSPENDABLE_H
#define COMMON_SUSPENDABLE_H

#include <memory>
#include <mutex>

namespace common
{
    /**
     * @brief Thread safe mechanic for flow controlled features
     * 
     */
    class Suspendable
    {
    public:
        /**
         * @brief Construct a new Suspendable object
         */
        Suspendable();

        /**
         * @brief Destroy the Suspendable object
         */
        ~Suspendable();

        /**
         * @brief Copy construct a new Suspendable object
         * @param to_copy Suspendable object to copy from
         */
        Suspendable(const Suspendable& to_copy);
        
        /**
         * @brief Move construct a new Suspendable object
         * @param to_move Suspendable object to move
         */
        Suspendable(Suspendable&& to_move);

        /**
         * @brief Copy assignment operator 
         * 
         * @param to_copy_assign RHS to copy from
         * @return Suspendable& LHS to assign to
         */
        Suspendable& operator=(const Suspendable& to_copy_assign);

        /**
         * @brief Move assignment operator
         * 
         * @param to_move_assign RHS to move from
         * @return Suspendable& LHS to assign to
         */
        Suspendable& operator=(Suspendable&& to_move_assign);
        
        /**
         * @brief suspends process of managed execution
         */
        void suspend();

        /**
         * @brief resumes process of managed execution
         */
        void resume();

        /**
         * @brief Gets state of managed execution
         * @return true if execution is suspended, else false
         */
        bool is_suspended();

    private:
        std::mutex lock_; //lock providing thread safety
        bool is_suspended_; //flow control boolean
    };
}

#endif //COMMON_SUSPENDABLE_H