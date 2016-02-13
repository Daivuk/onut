#pragma once
#include <mutex>
#include <queue>

#include "onut/Pool.h"

namespace onut
{
    class Synchronous
    {
    public:
        Synchronous()
        {
            m_pAllocator = OPool::create();
        }

        /**
        Synchronise to the calling thread.
        The function and arguments passed here, will be queued and called next time invokeQueue() is called.
        @param callback Function or your usual lambda
        @param args arguments
        */
        template<typename Tfn,
            typename ... Targs>
            void sync(Tfn callback, Targs... args)
        {
            m_mutex.lock();
            auto pCallback = m_pAllocator->alloc<Callback<Tfn, Targs...>>(callback, args...);
            syncCallback(pCallback);
            m_mutex.unlock();
        }

        /**
        Call all currently queued callbacks set using sync() calls.
        */
        void processQueue()
        {
            m_mutex.lock();
            while (!m_callbackQueue.empty())
            {
                auto pCallback = m_callbackQueue.front();
                m_callbackQueue.pop();

                // We unlock during the call. Because the call might add new callbacks!
                m_mutex.unlock();
                pCallback->call();
                m_mutex.lock();
                m_pAllocator->dealloc(pCallback);
            }
            m_mutex.unlock();
        }

    private:
        class ICallback
        {
        public:
            virtual void call() = 0;
        };

        template<typename ... Targs>
        class Callback : public ICallback
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

        std::queue<ICallback*>  m_callbackQueue;

    public:
        /**
        Get the count of callbacks currently in the queue
        */
        auto size() -> decltype(m_callbackQueue.size()) const
        {
            m_mutex.lock();
            auto ret = m_callbackQueue.size();
            m_mutex.unlock();
            return ret;
        }

    private:
        void syncCallback(ICallback* pCallback)
        {
            m_callbackQueue.push(pCallback);
        }

        std::mutex m_mutex;
        OPoolRef m_pAllocator;
    };
}
