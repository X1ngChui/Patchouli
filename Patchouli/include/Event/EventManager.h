#pragma once

#include "Event/Event.h"
#include "Event/EventBus.h"

namespace Patchouli
{
    // Forward declaration of EventHandlerBase class
    class EventHandlerBase;

    template <TypeEvent... Args>
    class EventHandler;

    // EventDispatcher class for managing event handlers and dispatching events
    class PATCHOULI_API EventManager : public PObject
    {
    public:
        EventManager() = default;

        virtual ~EventManager() = default;

        EventManager(const EventManager&) = delete;
        EventManager& operator=(const EventManager&) = delete;

        template <TypeEvent E>
        EventManager& addHandler(Ref<EventHandler<E>> handler)
        {
            addHandlerImpl(handler, E::getStaticType());
            return *this;
        }

        template <TypeEvent E, TypeEvent... Rest>
        EventManager& addHandler(Ref<EventHandler<E, Rest...>> handler)
        {
            addHandlerImpl(handler, E::getStaticType());
            return addHandler<Rest...>(std::static_pointer_cast<EventHandler<Rest...>>(handler));
        }

        template <TypeEvent E>
        EventManager& removeHandler(Ref<EventHandler<E>> handler)
        {
            removeHandlerImpl(handler, E::getStaticType());
            return *this;
        }

        template <TypeEvent E, TypeEvent... Rest>
        EventManager& removeHandler(Ref<EventHandler<E, Rest...>> handler)
        {
            removeHandlerImpl(handler, E::getStaticType());
            return removeHandler<Rest...>(std::static_pointer_cast<EventHandler<Rest...>>(handler));
        }

        // Publish an event to the event queue (thread-safety)
        template <TypeEvent E, typename... Args>
        EventManager& publish(Args&&... args)
        {
            publish(makeRef<E>(std::forward<Args>(args)...));
            return *this;
        }

        // Publish an event to the event queue (thread-safety)
        EventManager& publish(Ref<Event> event);

        // Publish an event to the event queue (thread-safety)
        EventManager& publish(std::initializer_list<Ref<Event>> event);

        // Start the event loop (blocking, no thread-safety)
        void run();

    private:
        // Add an event handler for a specific event type
        void addHandlerImpl(Ref<EventHandlerBase> handler, EventType type);

        // Remove an event handler for a specific event type
        void removeHandlerImpl(Ref<EventHandlerBase> handler, EventType type);

        void onEvent(Ref<Event> event);
        
        void onTerminationEvent();

    private:
        bool running = false; // Flag indicating whether the event loop is running

        std::mutex mapMutex; // Mutex for protecting the event handler map
        std::unordered_multimap<EventType, Ref<EventHandlerBase>> handlerMap; // Map of event type to a vector of event handlers

        EventBus eventBus;
    };
}