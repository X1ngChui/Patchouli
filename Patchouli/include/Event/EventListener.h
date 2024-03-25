#pragma once

#include "Event/Event.h"
#include "Event/EventDispatecher.h"

namespace Patchouli
{
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
        void operator()(Ref<Event> event) const
        {
            callback(event);
        }

        // Get the envent type id
        virtual EventType getType() const = 0;

    protected:
        friend class EventDispatcher;
        WeakRef<EventDispatcher> dispatcher = nullptr; // Weak reference to the event dispatcher

    private:
        EventCallback callback; // Callback function to handle events
    };

    // Template class for event listeners
    template <TypeEvent E>
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

        constexpr EventType getType() const override { return E::getStaticType(); }
    };
}