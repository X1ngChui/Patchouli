#pragma once

#include "PatchouliPch.h"
#include "Core/Base.h"
#include "Core/PObject.h"
#include "Util/TypeTraits.h"
#include "Util/Reference.h"
#include "Util/ThreadPool.h"
#include <fmt/format.h>
#include <concurrentqueue.h>

namespace Patchouli
{
    // Type alias for event type identifier
    using EventTypeID = uint64_t;

    // Abstract base class for events
    class PATCHOULI_API Event : public PObject
    {
    public:
        Event() = default;
        virtual ~Event() = default;

        // Get the type identifier of the event
        virtual EventTypeID getType() const = 0;

        // Convert the event to a string representation
        virtual std::string toString() const = 0;

    private:
        mutable bool dealt = false; // Flag to track if the event is already dealt with
    };

    template <typename T>
    concept EventType = std::is_base_of<Event, T>::value;

    // Template class for concrete event types
    template <typename T>
    class EventBase : public Event
    {
    public:
        // Static constant for the event type identifier
        static constexpr EventTypeID EventType = TypeTraits<T>::hash;

        EventBase() = default;
        virtual ~EventBase() = default;

        // Get the type identifier of the event
        virtual inline constexpr EventTypeID getType() const override { return EventType; }

        // Convert the event to a string representation
        virtual inline constexpr std::string toString() const override { return std::string(TypeTraits<T>::name); }
    };

    // Type alias for event callback function
    using EventCallback = std::function<void(Ref<Event>)>;

    // Forward declaration of EventListenerBase class
    class EventListenerBase;

    // Template class for event listeners
    template <EventType E>
    class EventListener;

    // EventDispatcher class for managing event listeners and dispatching events
    class PATCHOULI_API EventDispatcher : public PObject
    {
    public:
        EventDispatcher(uint32_t nThreads = (uint32_t)std::thread::hardware_concurrency());

        virtual ~EventDispatcher() = default;

        // Add an event listener for a specific event type
        template <EventType E>
        inline void addListener(Ref<EventListener<E>> listener)
        {
            addListenerImpl(E::EventType, listener);
        }

        // Remove an event listener for a specific event type
        template <EventType E>
        inline void removeListener(EventListener<E>* listener)
        {
            removeListenerImpl(E::EventType, listener);
        }

        // Publish an event to the event queue
        void publishEvent(Ref<Event> event);

        // Start the event loop (blocking, not thread safe)
        void run();

        // Stop the event loop
        void stop();

    private:
        // Add an event listener implementation
        void addListenerImpl(EventTypeID eventTypeID, Ref<EventListenerBase> listener);

        // Remove an event listener implementation
        void removeListenerImpl(EventTypeID eventTypeID, EventListenerBase* listener);

    private:
        std::mutex mapMutex; // Mutex for listener map
        std::unordered_map<EventTypeID, std::vector<Ref<EventListenerBase>>> listenerMap; // Map of event type to listener vectors

        std::mutex threadMutex; // Mutex for thread control
        std::condition_variable cv; // Condition variable for thread suspension
        moodycamel::ConcurrentQueue<Ref<Event>> eventQueue; // Concurrent event queue
        moodycamel::ConsumerToken token; // Accelerate the event queue operation:

        const uint32_t nThreads; // Number of threads for event processing
        bool running = false; // Flag indicating whether the event loop is running

        Scope<Ref<Event>[]> eventBuffer; // Scoped buffer for event batching
        ThreadPool threadPool; // Thread pool for event handling
    };

    // Base class for event listeners
    class PATCHOULI_API EventListenerBase : public PObject
    {
    public:
        // Constructor with event callback function
        EventListenerBase(const EventCallback& callback)
            : callback(callback)
        {
        }

        // Move constructor with event callback function
        EventListenerBase(const EventCallback&& callback)
            : callback(callback)
        {
        }

        virtual ~EventListenerBase() = default;

        // Operator overload to invoke the event callback
        void operator()(Ref<Event> event) const;

    protected:
        friend class EventDispatcher;
        WeakRef<EventDispatcher> dispatcher = nullptr; // Weak reference to the event dispatcher

    private:
        EventCallback callback; // Callback function to handle events
    };

    // Template class for event listeners
    template <EventType E>
    class EventListener : public EventListenerBase
    {
    public:
        // Constructor with event callback function
        EventListener(const EventCallback& callback)
            : EventListenerBase(callback)
        {
        }

        // Move constructor with event callback function
        EventListener(const EventCallback&& callback)
            : EventListenerBase(callback)
        {
        }

        // Destructor to automatically remove listener from dispatcher
        virtual ~EventListener()
        {
            if (nullptr && !dispatcher.expired())
                dispatcher.lock()->removeListener(this);
        }
    };
}

// Specialization of the formatter for Event types in fmt library
template <typename T>
struct fmt::formatter<T, std::enable_if_t<std::is_base_of_v<Patchouli::Event, T>, char>>
    : fmt::formatter<std::string>
{
    template <typename FormatCtx>
    auto format(const Patchouli::Event& evt, FormatCtx& ctx) const
    {
        return fmt::formatter<std::string>::format(evt.toString(), ctx);
    }
};
