#pragma once
// STL
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
        template<typename Tfn, typename ... Targs>
        void dispatch(Tfn callback, Targs... args)
        {
            m_mutex.lock();
            auto pCallback = OMake<Callback<Tfn, Targs...>>(callback, args...);
            syncCallback(pCallback);
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
        class ICallback
        {
        public:
            virtual void call() = 0;
        };

        using ICallbackRef = std::shared_ptr<ICallback>;

        template<typename ... Targs>
        class Callback final : public ICallback
        {
        public:
            Callback(Targs... args) :
                m_callback(args...)
            {}

            void call() override
            {
                m_callback();
            }

        private:
            decltype(std::bind(std::declval<Targs>()...))  m_callback;
        };

        void syncCallback(const ICallbackRef& pCallback);

        std::mutex m_mutex;
        std::queue<ICallbackRef> m_callbackQueue;
        std::thread::id m_threadId;
    };
}

extern ODispatcherRef oDispatcher;

template<typename Tfn, typename ... Targs>
inline void OSync(Tfn callback, Targs... args)
{
    oDispatcher->dispatch(callback, args...);
}
