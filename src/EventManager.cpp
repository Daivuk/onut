#include "EventManager.h"

namespace onut
{
    EventObserver::EventObserver(EventManager* pEventManager) :
        m_pEventManager(pEventManager)
    {}

    EventObserver::~EventObserver()
    {
        stopObservingAll();
    }

    void EventObserver::observe(const std::string& eventName, const std::function<void()>& callback)
    {
        if (m_pEventManager)
        {
            m_pEventManager->addObserver(eventName, this, callback);
        }
    }

    void EventObserver::stopObserving(const std::string& eventName)
    {
        if (m_pEventManager)
        {
            m_pEventManager->removeObserver(eventName, this);
        }
    }

    void EventObserver::stopObservingAll()
    {
        if (m_pEventManager)
        {
            m_pEventManager->removeObserver(this);
        }
    }

    EventManager::~EventManager()
    {
        for (auto& kv : m_observers)
        {
            for (auto& observer : kv.second)
            {
                observer.pObserver->m_pEventManager = nullptr;
            }
        }
    }

    void EventManager::addEvent(const std::string& eventName, const std::function<bool()>& eventProcessor)
    {
        m_processors.push_back({eventName, eventProcessor});
    }

    void EventManager::removeEvent(const std::string& eventName)
    {
        for (auto it = m_processors.begin(); it != m_processors.end(); ++it)
        {
            if (it->name == eventName)
            {
                m_processors.erase(it);
                break;
            }
        }
    }

    void EventManager::clear()
    {
        m_processors.clear();
    }

    void EventManager::addObserver(const std::string& eventName, EventObserver* pObserver, const std::function<void()>& callback)
    {
        m_observers[eventName].push_back({pObserver, callback});
    }

    void EventManager::removeObserver(const std::string& eventName, EventObserver* pObserver)
    {
        if (m_observers.count(eventName))
        {
            auto& observers = m_observers[eventName];
            auto end = observers.end();
            for (auto it = observers.begin(); it != end;)
            {
                auto& observer = *it;
                if (observer.pObserver == pObserver)
                {
                    it = observers.erase(it);
                    end = observers.end();
                    continue;
                }
                ++it;
            }
        }
    }

    void EventManager::removeObserver(EventObserver* pObserver)
    {
        for (auto& kv : m_observers)
        {
            auto& observers = kv.second;
            auto end = observers.end();
            for (auto it = observers.begin(); it != end;)
            {
                auto& observer = *it;
                if (observer.pObserver == pObserver)
                {
                    it = observers.erase(it);
                    end = observers.end();
                    continue;
                }
                ++it;
            }
        }
    }

    void EventManager::processEvents()
    {
        for (auto& processor : m_processors)
        {
            if (processor.eventProcessor())
            {
                fireEvent(processor.name);
            }
        }
    }

    void EventManager::fireEvent(const std::string& eventName)
    {
        if (m_observers.count(eventName))
        {
            auto& observers = m_observers[eventName];
            for (auto& observer : observers)
            {
                observer.callback();
            }
        }
    }
}
