// onut
#include <onut/ThreadPool.h>

OThreadPoolRef oThreadPool;

namespace onut
{
    OThreadPoolRef OThreadPool::create()
    {
        return std::shared_ptr<ThreadPool>(new ThreadPool());
    }

    void ThreadPool::workerThread(ThreadPool::WorkerRef pWorker)
    {
        std::unique_lock<std::mutex> lock(pWorker->mutex);
        while (pWorker->isRunning)
        {
            pWorker->waitForWork.wait(lock);
            if (!pWorker->isRunning)
            {
                break;
            }
            pWorker->pDispatcher->processQueue();
        }
    }

    ThreadPool::ThreadPool()
        : m_nextWorker(0)
    {
        auto threadCount = std::thread::hardware_concurrency();
        for (decltype(threadCount) i = 0; i < threadCount; ++i)
        {
            WorkerRef pWorker = OMake<Worker>();
            pWorker->pDispatcher = OMake<ODispatcher>();
            pWorker->thread = std::thread(workerThread, pWorker);
            m_workers.push_back(pWorker);
        }
    }

    ThreadPool::~ThreadPool()
    {
        for (auto& pWorker : m_workers)
        {
            {
                std::unique_lock<std::mutex> lock(pWorker->mutex);
                pWorker->isRunning = false;
                pWorker->waitForWork.notify_all();
            }
            pWorker->thread.join();
        }
    }

    void ThreadPool::wait()
    {
        for (auto& pWorker : m_workers)
        {
            std::unique_lock<std::mutex> lock(pWorker->mutex);
        }
        m_nextWorker = 0;
    }
}
