#pragma once

#include "PatchouliPch.h"
#include "Core/Base.h"
#include "Util/TypeTraits.h"
#include "Util/Reference.h"
#include <fmt/format.h>
#include <concurrentqueue.h>

namespace Patchouli
{
    // Type alias for event type identifier
    using EventTypeID = uint64_t;

    // Abstract base class for events
    class PATCHOULI_API Event : public RefBase<Event>
    {
    public:
        Event() = default;
        virtual ~Event() = default;

        // Get the type identifier of the event
        virtual EventTypeID getType() const = 0;

        // Convert the event to a string representation
        virtual std::string toString() const = 0;

        // Overloaded new operators for memory management
        void* operator new(std::size_t size);
        void* operator new[](std::size_t size);
        void* operator new(std::size_t size, const std::nothrow_t& tag) noexcept;
        void* operator new[](std::size_t size, const std::nothrow_t& tag) noexcept;

        // Overloaded delete operators for memory management
        void operator delete(void* ptr);
        void operator delete[](void* ptr);
        void operator delete(void* ptr, const std::nothrow_t& tag) noexcept;
        void operator delete[](void* ptr, const std::nothrow_t& tag) noexcept;
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
    class PATCHOULI_API EventDispatcher : public RefBase<EventDispatcher>
    {
    public:
        EventDispatcher();

        ~EventDispatcher();

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

    private:
        // Add an event listener implementation
        void addListenerImpl(EventTypeID eventTypeID, Ref<EventListenerBase> listener);

        // Remove an event listener implementation
        void removeListenerImpl(EventTypeID eventTypeID, EventListenerBase* listener);

        // Dispatch all the events in the event queue
        void dispatch();

        // Main loop for event dispatching
        void dispatchLoop();

    private:
        std::mutex mapMutex; // Mutex for listener map
        std::unordered_map<EventTypeID, std::vector<Ref<EventListenerBase>>> listenerMap; // Map of event type to listener vectors

        moodycamel::ConcurrentQueue<Ref<Event>> eventQueue; // Concurrent event queue

        bool running = true; // Flag indicating whether the event dispatcher is running
        std::mutex threadMutex; // Mutex for dispatcher thread synchronization
        std::condition_variable cv; // Condition variable for dispatcher thread
        std::thread dispatcherThread; // Dispatcher thread
    };

    // Base class for event listeners
    class PATCHOULI_API EventListenerBase : public RefBase<EventListenerBase>
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
