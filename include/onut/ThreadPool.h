#pragma once
// onut
#include <onut/Dispatcher.h>

// STL
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(ThreadPool)

namespace onut
{
    class ThreadPool final
    {
    public:
        static OThreadPoolRef create();

        ~ThreadPool();

        template<typename ... Targs>
        void doWork(Targs ... args)
        {
            auto pWorker = m_workers[m_nextWorker];
            std::unique_lock<std::mutex> lock(pWorker->mutex);
            pWorker->pDispatcher->dispatch(args ...);
            pWorker->waitForWork.notify_one();
            m_nextWorker = (m_nextWorker + 1) % m_workers.size();
        }

        void wait();

    private:
        struct Worker
        {
            std::mutex mutex;
            std::condition_variable waitForWork;
            std::thread thread;
            bool isRunning = true;
            ODispatcherRef pDispatcher;
        };
        using WorkerRef = std::shared_ptr<Worker>;
        using Workers = std::vector<WorkerRef>;

        ThreadPool();

        static void workerThread(WorkerRef pWorker);

        Workers::size_type m_nextWorker;
        Workers m_workers;
    };
}

extern OThreadPoolRef oThreadPool;

#define OWork(...) oThreadPool->doWork(__VA_ARGS__)
#define OWait oThreadPool->wait
