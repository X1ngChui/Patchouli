#pragma once

#include "Event/Event.h"
#include "Event/EventManager.h"

namespace Patchouli
{
    class PATCHOULI_API EventHandler : public PObject
    {
    public:
        EventHandler(EventCallback&& callback)
            : callback(std::move(callback))
        {
        }

        ~EventHandler() = default;

        // Operator overload to invoke the event callback
        void operator()(Ref<Event> event) const
        {
            callback(event);
        }

    private:
        EventCallback callback; // Callback function to handle events
    };
}