#pragma once

#include "Event/Event.h"
#include "Event/EventManager.h"

namespace Patchouli
{
    class EventHandlerBase : public PObject
    {
    public:
        EventHandlerBase(EventCallback&& callback)
            : callback(std::move(callback))
        {
        }

        virtual ~EventHandlerBase() = default;

        // Operator overload to invoke the event callback
        void operator()(Ref<Event> event) const
        {
            callback(event);
        }

    protected:
        EventCallback callback; // Callback function to handle events
    };

    template <TypeEvent... Args>
    class EventHandler;

    template <TypeEvent E>
    class EventHandler<E> : public EventHandlerBase
    {
    public:
        EventHandler(EventCallback&& callback)
            : EventHandlerBase(std::move(callback))
        {
        }
    };

    template <TypeEvent E, TypeEvent... Rest>
    class EventHandler<E, Rest...> : public EventHandler<Rest...>
    {
    public:
        EventHandler(EventCallback&& callback)
            : EventHandler<Rest...>(std::move(callback))
        {
        }
    };
}