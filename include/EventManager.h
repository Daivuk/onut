#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace onut {
    class EventManager {
    public:
        void addEvent(const std::string& eventName, const std::function<bool()>& eventProcessor);
        void removeEvent(const std::string& eventName);

        void addObserver(const std::string& eventName, const std::function<void()>& callback);
        void removeObserver(const std::string& eventName, const std::function<void()>& callback);

        void update();
        void fireEvent(const std::string& eventName);

    private:
        struct sEventProcessor {
            std::string                name;
            std::function<bool()>    eventProcessor;
        };

        std::vector<sEventProcessor>                                        m_processors;
        std::unordered_map<std::string, std::vector<std::function<void()>>>    m_observers;
    };
}
