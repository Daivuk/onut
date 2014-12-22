#pragma once
#include <mutex>

namespace onut {
    /**
        Default allocator for Synchronous class.
    */
    class SynchronousDefaultAllocator {
    public:
        template<typename Ttype, typename ... Targs>
        Ttype* alloc(Targs... args) {
            return new Ttype(args...);
        }

        void dealloc(void* pObj) {
            delete pObj;
        }
    };

    /**
        Helper class to run function callbacks back to the calling thread. This also can use a custom allocator
        template arguments:
        - Tallocator: Allocator used for callbacks. Each time a callback is set, it's allocated. And destroyed after called. It could be beneficial for a game to use a pool. As long as your custom allocator define alloc<T>() and dealloc() method, you should be good.
    */
    template<typename Tallocator = SynchronousDefaultAllocator>
    class Synchronous {
    public:
        /**
            Synchronise to the calling thread.
            The function and arguments passed here, will be queued and called next time invokeQueue() is called.
            @param callback Function or your usual lambda
            @param args arguments
        */
        template<typename Tfn, typename ... Targs>
        void sync(Tfn callback, Targs... args) {
            auto pCallback = m_allocator.alloc<Callback<Tfn, Targs...>>(callback, args...);
            syncCallback(pCallback);
        }

        /**
            Call all currently queued callbacks set using sync() calls.
        */
        void invokeQueue() {
            m_mutex.lock();
            while (!m_callbackQueue.empty()) {
                auto pCallback = m_callbackQueue.front();
                m_callbackQueue.pop();

                // We unlock during the call. Because the call might add new callbacks!
                m_mutex.unlock();
                pCallback->call();
                m_allocator.dealloc(pCallback);
                m_mutex.lock();
            }
            m_mutex.unlock();
        }

    private:
        class ICallback {
        public:
            virtual void call() = 0;
        };

        template<typename ... Targs>
        class Callback : public ICallback {
        public:
            Callback(Targs... args) :
                m_callback(args...)  {
            }

            void call() override {
                m_callback();
            }

        private:
            decltype(std::bind(std::declval<Targs>()...))  m_callback;
        };

        void syncCallback(ICallback* pCallback) {
            m_mutex.lock();
            m_callbackQueue.push(pCallback);
            m_mutex.unlock();
        }

        std::queue<ICallback*>  m_callbackQueue;
        std::mutex              m_mutex;
        Tallocator              m_allocator;
    };
}
