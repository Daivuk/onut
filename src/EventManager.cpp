#include "EventManager.h"

namespace onut {
    void EventManager::addEvent(const std::string& eventName, const std::function<bool()>& eventProcessor) {
        m_processors.push_back({ eventName, eventProcessor });
    }

    void EventManager::removeEvent(const std::string& eventName) {
        for (auto it = m_processors.begin(); it != m_processors.end(); ++it) {
            if (it->name == eventName) {
                m_processors.erase(it);
                break;
            }
        }
    }

    void EventManager::addObserver(const std::string& eventName, const std::function<void()>& callback) {
        m_observers[eventName].push_back(callback);
    }

    void EventManager::removeObserver(const std::string& eventName, const std::function<void()>& callback) {
        if (m_observers.count(eventName)) {
            auto& observers = m_observers[eventName];
            auto end = observers.end();
            for (auto it = observers.begin(); it != end;) {
                auto& observer = *it;
                if (observer.target_type() == callback.target_type() &&
                    observer.target<void()>() == callback.target<void()>()) {
                    it = observers.erase(it);
                    if (observers.empty()) {
                        m_observers.erase(eventName);
                        return;
                    }
                    end = observers.end();
                    continue;
                }
                ++it;
            }
        }
    }

    void EventManager::update() {
        for (auto& processor : m_processors) {
            if (processor.eventProcessor()) {
                fireEvent(processor.name);
            }
        }
    }

    void EventManager::fireEvent(const std::string& eventName) {
        if (m_observers.count(eventName)) {
            auto& observers = m_observers[eventName];
            auto end = observers.end();
            for (auto it = observers.begin(); it != end; ++it) {
                auto& observer = *it;
                observer();
            }
        }
    }
}
