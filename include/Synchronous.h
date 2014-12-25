#pragma once
#include <mutex>
#include <queue>

namespace onut
{
    /**
    Default allocator for Synchronous class.
    */
    class SynchronousDefaultAllocator
    {
    public:
        template<typename Ttype,
            typename ... Targs>
            Ttype* alloc(Targs... args) const
        {
            return new Ttype(args...);
        }

        void dealloc(void* pObj) const
        {
            delete pObj;
        }
    };

    /**
    Helper class to run function callbacks back to the calling thread. This also can use a custom allocator
    template arguments:
    - Tallocator: Allocator used for callbacks. Each time a callback is set, it's allocated. And destroyed after called. It could be beneficial for a game to use a pool. As long as your custom allocator defines alloc<T>() and dealloc() method, you should be good.
    - TmutexType: Mutex type to be used. Default std::mutex.
    */
    template<typename Tallocator = SynchronousDefaultAllocator,
        typename TmutexType = std::mutex>
    class Synchronous
    {
    public:
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
            auto pCallback = m_allocator.alloc<Callback<Tfn, Targs...>>(callback, args...);
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
                m_allocator.dealloc(pCallback);
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

        TmutexType  m_mutex;
        Tallocator  m_allocator;
    };
}
