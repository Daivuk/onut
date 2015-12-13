#pragma once
#include <future>
#include "Pool.h"
#include "Synchronous.h"

/**
Synchronize back to main thread. This can also be called from the main thread. It will just be delayed until the next frame.
@param callback Function or your usual lambda
@param args arguments
*/
template<typename Tfn,
    typename ... Targs>
    inline void OSync(Tfn callback, Targs... args)
{
    extern onut::Synchronous<onut::Pool<>> g_mainSync;
    g_mainSync.sync(callback, args...);
}


/**
    Run a task asynchronously from the current thread.
    @param args Function and arguments, or your usual lambda
    @return std::future<> of the type of your functions
    @note This is the same as calling std::async(std::launch::async, ...). It's just shorter and use the default async launch policy.
*/
template<typename ... Targs>
inline auto OAsync(Targs... args) -> decltype(std::async(args...))
{
    return std::async(std::launch::async, args...);
}

template<typename TasyncWork, typename TsyncWork>
inline void OSequencialWork(TasyncWork asyncWork, TsyncWork syncWork)
{
    OAsync([=]
    {
        asyncWork();
        OSync([=]
        {
            syncWork();
        });
    });
}

template<typename TasyncWork>
inline void OSequencialWork(TasyncWork asyncWork)
{
    OAsync([=]
    {
        asyncWork();
    });
}

/**
* @fn So async/sync/async/sync calls sequentially
* Sometimes while loading you need to sync back to main loop. Then continue loading more stuff asynchronously.
* First argument is a function or lambda to async work. Then the work is done, it will call the second call synchronously. Then the second one is done, it will call the third one asynchronously, etc.
*/
template<typename TasyncWork, typename TsyncWork, typename ... Targs>
inline void OSequencialWork(TasyncWork asyncWork, TsyncWork syncWork, Targs... args)
{
    OAsync([=]
    {
        asyncWork();
        OSync([=]
        {
            syncWork();
            OSequencialWork(args...);
        });
    });
}

namespace onut
{
    class TasksRunner
    {
    private:
        std::vector<std::function<void()>>              m_tasks;
        std::vector<std::function<void()>>::size_type   m_index = 0;
        std::mutex                                      m_mutex;

        static void runTasks(TasksRunner& taskRunner)
        {
            decltype(taskRunner.m_index) size = taskRunner.m_tasks.size();
            taskRunner.m_mutex.lock();
            while (taskRunner.m_index < size)
            {
                auto fn = taskRunner.m_tasks[taskRunner.m_index++];
                taskRunner.m_mutex.unlock();
                fn();
                taskRunner.m_mutex.lock();
            }
            taskRunner.m_mutex.unlock();
        }

    public:
        TasksRunner(const decltype(m_tasks)& tasks) :
            m_tasks(tasks)
        {
        }

        void run()
        {
            std::vector<std::future<void>> threads;
            auto threadCount = std::thread::hardware_concurrency() - 1;
            for (decltype(threadCount) i = 0; i < threadCount; ++i)
            {
                threads.push_back(std::async(std::launch::async, [this]{
                    runTasks(*this);
                }));
            }
            runTasks(*this);
            for (auto& thread : threads)
            {
                thread.wait();
            }
        }
    };
}

inline void ORunTasks(const std::vector<std::function<void()>>& tasks)
{
    onut::TasksRunner taskRunner(tasks);
    taskRunner.run();
}
