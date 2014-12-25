#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace onut {
    class EventManager;

    class EventObserver {
    public:
        EventObserver(EventManager* pEventManager);
        virtual ~EventObserver();

        void observe(const std::string& eventName, const std::function<void()>& callback);
        void stopObserving(const std::string& eventName);
        void stopObservingAll();

    private:
        EventManager* m_pEventManager;
    };

    class EventManager {
    private:
        struct sEventProcessor {
            std::string             name;
            std::function<bool()>   eventProcessor;
        };

        struct sObserver {
            EventObserver*          pObserver;
            std::function<void()>   callback;
        };

        std::vector<sEventProcessor>                            m_processors;
        std::unordered_map<std::string, std::vector<sObserver>> m_observers;

    public:
        void addEvent(const std::string& eventName, const std::function<bool()>& eventProcessor);
        void removeEvent(const std::string& eventName);
        void clear();
        auto size() -> decltype(m_processors.size()) const { return m_processors.size(); }

        void addObserver(const std::string& eventName, EventObserver* pObserver, const std::function<void()>& callback);
        void removeObserver(const std::string& eventName, EventObserver* pObserver);
        void removeObserver(EventObserver* pObserver);

        void processEvents();
        void fireEvent(const std::string& eventName);
    };
}

typedef onut::EventObserver OObserver;
