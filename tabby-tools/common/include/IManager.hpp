#ifndef COMMON_IMANAGER_HPP_
#define COMMON_IMANAGER_HPP_

#include <memory>

namespace common
{
    /**
     * @brief Enumeration detailing state of managed feature
     * 
     */
    enum ManagedState
    {
        Fault=-1,
        Uninitialized,
        Running,
        Suspended,
        Stopped,
        Terminated
    };

    /**
     * @brief Generic interface providing controls for managed features.
     */
    class IManager
    {
    public:
        
        /**
         * @brief interface method to start the managed feature.
         * 
         */
        virtual void start() = 0;

        /**
         * @brief interface method to stop the managed feature.
         * 
         */
        virtual void stop() = 0;

        /**
         * @brief interface method to pause the managed feature.
         * 
         */
        virtual void pause() = 0;

        /**
         * @brief interface method to resume the managed feature.
         */
        virtual void resume() = 0;

        /**
         * @brief interface method returns the current managed state
         * @return ManagedState 
         */
        virtual ManagedState state() = 0;
    };
}

#endif //COMMON_IMANAGER_HPP_