#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace onut
{
    class EventManager;

    /**
    Observes events in an EventManager
    @note This class is not thread safe
    */
    class EventObserver
    {
    public:
        /**
        Constructor
        @param pEventManager The event manager to observe.
        */
        EventObserver(EventManager* pEventManager);

        /**
        Destructor
        stopObservingAll() is called
        */
        virtual ~EventObserver();

        /**
        Observe an event.
        @param eventName Name of the event created in the EventManager
        @param callback Callback to call if the event is fired
        */
        void observe(const std::string& eventName, const std::function<void()>& callback);

        /**
        Stop observing a particular event
        @param eventName Name of the event to stop listening to
        */
        void stopObserving(const std::string& eventName);

        /**
        Stop all obeserving
        */
        void stopObservingAll();

    private:
        friend EventManager;

        EventManager* m_pEventManager;
    };

    /**
    Manage and fire events
    @note This class is not thread safe
    */
    class EventManager
    {
    private:
        struct sEventProcessor
        {
            std::string             name;
            std::function<bool()>   eventProcessor;
        };

        struct sObserver
        {
            EventObserver*          pObserver;
            std::function<void()>   callback;
        };

        std::vector<sEventProcessor>                            m_processors;
        std::unordered_map<std::string, std::vector<sObserver>> m_observers;

    public:
        /**
        * Will remove all observers
        */
        virtual ~EventManager();

        /**
        Add an event to be processed when processEvents() is called.
        @param eventName Name of the event. If the name is already present, previous one will be overriden
        @param eventProcessor Function or lambda that returns true if the event meet the requirements.
        */
        void addEvent(const std::string& eventName, const std::function<bool()>& eventProcessor);

        /**
        Remove event from being processed when processEvents is called.
        @param eventName Name of the event to stop processing
        @note Observers that are observing this event are not removed. So if the event is added back
        they will start receiving callbacks again.
        */
        void removeEvent(const std::string& eventName);

        /**
        Remove all events from being processed
        @note Observers that are observing are not removed. So if events are added back
        they will start receiving callbacks again.
        */
        void clear();

        /**
        Get the event count in the Manager.
        */
        auto size() -> decltype(m_processors.size()) const { return m_processors.size(); }

        /**
        Observe an event
        @param eventName Name of the event to observe
        @pObserver Observer object registering to this event. This is required for safety because the std::function callback can't remove itself if the observer is destroyed
        @callback Function to call when the event is fired.
        */
        void addObserver(const std::string& eventName, EventObserver* pObserver, const std::function<void()>& callback);

        /**
        Stop observing an event for a particular observer
        @param eventName Name of the event to stop oberserving
        @param pObserver Observer to unregister from that event
        */
        void removeObserver(const std::string& eventName, EventObserver* pObserver);

        /**
        Stop observing all events that an observer is observing
        @param pObserver The observer to unregister entirely from the EventManager
        */
        void removeObserver(EventObserver* pObserver);

        /**
        Process events. Call this every frame or so.
        This loops all events and call their processor. If the processor returns true, the event is fired
        */
        void processEvents();

        /**
        Manually fire an event by name.
        @note This does not require the event to be registered. It goes straight to the observers and call their callbacks.
        */
        void fireEvent(const std::string& eventName);
    };
}

typedef onut::EventObserver OObserver;
