#pragma once

#include "Event/Event.h"

namespace Patchouli
{
    // Forward declaration of EventListenerBase class
    class EventListenerBase;

    // EventDispatcher class for managing event listeners and dispatching events
    class PATCHOULI_API EventDispatcher : public PObject
    {
    public:
        EventDispatcher(uint32_t nThreads = (uint32_t)std::thread::hardware_concurrency());

        virtual ~EventDispatcher() = default;

        EventDispatcher(const EventDispatcher&) = delete;
        EventDispatcher& operator=(const EventDispatcher&) = delete;

        // Add an event listener for a specific event type
        void addListener(Ref<EventListenerBase> listener);

        // Remove an event listener for a specific event type
        void removeListener(Ref<EventListenerBase> listener);

        // Publish an event to the event queue (thread-safety)
        void publishEvent(Ref<Event> event);

        // Publish events to the event queue (thread-safety)
        void publishEvents(const std::vector<Ref<Event>>& events);

        // Start the event loop (blocking, no thread-safety)
        void run();

    private:
        // Friend class for access fuction `removeListener`
        template <TypeEvent E>
        friend class EventListener;

        // Function called when a event begin
        inline void beginEvent();

        // Function called when a event end
        inline void endEvent();

        inline void onTerminationEvent();

        inline void onFenceEvent();

    private:
        bool running = false; // Flag indicating whether the event loop is running
        std::atomic<uint32_t> nTasks = 0; // Counter for events in process

        std::mutex loopMutex; // Mutex for event loop control
        std::condition_variable loopCv; // Condition variable for event loop suspension

        std::mutex mapMutex; // Mutex for protecting the event listener map
        // Map of event type to a vector of event listeners
        std::unordered_multimap<EventType, Ref<EventListenerBase>> listenerMap;

        moodycamel::BlockingConcurrentQueue<Ref<Event>> eventQueue; // Concurrent event queue
        moodycamel::ConsumerToken cToken; // Consumer token for event queue

        ThreadPool threadPool; // Thread pool for event handling
    };
}