#pragma once

#include "PatchouliPch.h"
#include "Core/Base.h"
#include "Core/PObject.h"
#include "Util/TypeTraits.h"
#include "Util/Reference.h"
#include "Util/ThreadPool.h"
#include "Util/Semaphore.h"
#include <fmt/format.h>
#include <blockingconcurrentqueue.h>

namespace Patchouli
{
    // Type alias for event type identifier
    using EventTypeID = uint64_t;

    // Abstract base class for events
    class PATCHOULI_API Event : public PObject
    {
    public:
        enum class ExecutionThread
        {
            Main = 0, Background
        };

        Event() = default;

        virtual ~Event() = default;

        // Get the type identifier of the event
        virtual EventTypeID getType() const = 0;

        // Get the event execution thread
        virtual ExecutionThread getExecutionThread() const = 0;

        // Convert the event to a string representation
        virtual std::string toString() const = 0;
    private:
        ExecutionThread executionThread;
    };

    template <typename T>
    concept EventType = std::is_base_of_v<Event, T> && std::is_final_v<T>;

    // Template class for concrete event types
    template <typename T, Event::ExecutionThread E = Event::ExecutionThread::Background>
    class EventBase : public Event
    {
    public:
        // Static constant for the event type identifier
        static constexpr EventTypeID EventType = TypeTraits<T>::hash;

        EventBase() { static_assert(std::is_final_v<T>, "This class is only for final class"); }
        virtual ~EventBase() = default;

        // Get the type identifier of the event
        virtual inline constexpr EventTypeID getType() const override { return EventType; }

        // Convert the event to a string representation
        virtual inline constexpr std::string toString() const override { return std::string(TypeTraits<T>::name); }

        virtual inline constexpr Event::ExecutionThread getExecutionThread() const override { return E; }
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

        EventDispatcher(const EventDispatcher&) = delete;
        EventDispatcher& operator=(const EventDispatcher&) = delete;

        // Add an event listener for a specific event type
        void addListener(Ref<EventListenerBase> listener);

        // Remove an event listener for a specific event type
        void removeListener(Ref<EventListenerBase> listener);

        // Publish an event to the event queue
        void publishEvent(Ref<Event> event);

        // Publish the events to the event queue
        void publishEvents(const std::vector<Ref<Event>>& events);

        // Start the event loop (blocking, not thread safe)
        void run();

    private:
        // Function called when a event begin
        void beginEvent();

        // Function called when a event end
        void endEvent();

        moodycamel::ProducerToken& getProducerToken();

        moodycamel::ConsumerToken& getConsumerToken();

    private:
        bool running = false; // Flag indicating whether the event loop is running
        std::atomic<uint32_t> nTasks; // Counter for events in process

        std::mutex loopMutex; // Mutex for event loop control
        std::condition_variable loopCv; // Condition variable for event loop suspension

        std::mutex mapMutex; // Mutex for protecting the event listener map
        // Map of event type to a vector of event listeners
        std::multimap<EventTypeID, Ref<EventListenerBase>> listenerMap;

        moodycamel::BlockingConcurrentQueue<Ref<Event>> eventQueue; // Concurrent event queue

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

        // Get the envent type id
        virtual EventTypeID getType() const = 0;

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
            if (!dispatcher.expired())
            {
                auto ref = dispatcher.lock();
                if (ref.get() != nullptr)
                    ref->removeListener(std::static_pointer_cast<EventListenerBase>(shared_from_this()));
            }
        }

        EventTypeID getType() const override { return E::EventType; }
    };
}

// Specialization of the formatter for Event types in fmt library
template <typename T>
struct fmt::formatter<T, std::enable_if_t<std::is_base_of_v<Patchouli::Event, T>, char>> : fmt::formatter<std::string>
{
    template <typename FormatCtx>
    auto format(const Patchouli::Event& evt, FormatCtx& ctx) const
    {
        return fmt::formatter<std::string>::format(evt.toString(), ctx);
    }
};
