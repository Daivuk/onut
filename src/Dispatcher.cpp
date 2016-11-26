// Onut
#include <onut/Dispatcher.h>

ODispatcherRef oDispatcher;

namespace onut
{
    ODispatcherRef Dispatcher::create()
    {
        return OMake<Dispatcher>();
    }

    Dispatcher::Dispatcher()
    {
        m_threadId = std::this_thread::get_id();
    }

    void Dispatcher::processQueue()
    {
        m_threadId = std::this_thread::get_id();
        m_mutex.lock();
        while (!m_callbackQueue.empty())
        {
            auto callback = m_callbackQueue.front();
            m_callbackQueue.pop();
            m_mutex.unlock();
            callback();
            m_mutex.lock();
        }
        m_mutex.unlock();
    }

    size_t Dispatcher::size()
    {
        m_mutex.lock();
        auto ret = m_callbackQueue.size();
        m_mutex.unlock();
        return ret;
    }

    std::thread::id Dispatcher::getThreadId() const
    {
        return m_threadId;
    }
}
