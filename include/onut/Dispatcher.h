#ifndef DISPATCHER_H_INCLUDED
#define DISPATCHER_H_INCLUDED

// STL
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(Dispatcher)

namespace onut
{
    class Dispatcher
    {
    public:
        static ODispatcherRef create();

        Dispatcher();

        /**
        Synchronise to the calling thread.
        The function and arguments passed here, will be queued and called next time invokeQueue() is called.
        @param callback Function or your usual lambda
        @param args arguments
        */
        void dispatch(const std::function<void()>& fn)
        {
            m_mutex.lock();
            m_callbackQueue.push(fn);
            m_mutex.unlock();
        }

        /**
        Call all currently queued callbacks set using sync() calls.
        */
        void processQueue();

        /**
        Get the count of callbacks currently in the queue
        */
        size_t size();

        std::thread::id getThreadId() const;

    private:
        std::mutex m_mutex;
        std::queue<std::function<void()>> m_callbackQueue;
        std::thread::id m_threadId;
    };
}

extern ODispatcherRef oDispatcher;

inline void OSync(const std::function<void()>& callback)
{
    oDispatcher->dispatch(callback);
}

#endif
