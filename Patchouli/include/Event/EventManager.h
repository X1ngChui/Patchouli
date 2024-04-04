#pragma once

#include "Event/Event.h"
#include <blockingconcurrentqueue.h>

namespace Patchouli
{
    // Forward declaration of EventHandler class
    class EventHandler;

    // EventDispatcher class for managing event handlers and dispatching events
    class PATCHOULI_API EventManager : public PObject
    {
    public:
        EventManager() = default;

        virtual ~EventManager() = default;

        EventManager(const EventManager&) = delete;
        EventManager& operator=(const EventManager&) = delete;

        // Add an event handler for a specific event type
        void addHandler(Ref<EventHandler> handler, EventType type);

        // Remove an event handler for a specific event type
        void removeHandler(Ref<EventHandler> handler);

        // Publish an event to the event queue (thread-safety)
        void publishEvent(Ref<Event> event);

        // Start the event loop (blocking, no thread-safety)
        void run();

    private:
        void onEvent(Ref<Event>& event);
        
        void onTerminationEvent();

    private:
        bool running = false; // Flag indicating whether the event loop is running

        std::mutex mapMutex; // Mutex for protecting the event handler map
        // Map of event type to a vector of event handlers
        std::unordered_multimap<EventType, Ref<EventHandler>> handlerMap;

        moodycamel::BlockingConcurrentQueue<Ref<Event>> eventQueue; // Concurrent event queue
    };
}