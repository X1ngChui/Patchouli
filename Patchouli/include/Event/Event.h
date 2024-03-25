#pragma once

#include "PatchouliPch.h"
#include "Core/Base.h"
#include "Core/PObject.h"
#include "Util/TypeTraits.h"
#include "Util/Reference.h"
#include "Util/ThreadPool.h"
#include <fmt/format.h>
#include <blockingconcurrentqueue.h>

namespace Patchouli
{
    // Type alias for event type identifier
    enum class EventType: uint32_t
    {
        None = 0,
        // Control Events
        Termination, Fence,

        // Application Events
        AppUpdate, AppRender,

        // Keyboard Events
        KeyPressed, KeyReleased, KeyTyped,

        // Mouse Events
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,

        // Window Events
        WindowUpdate, WindowClose, WindowResize
    };

    // Abstract base class for events
    class PATCHOULI_API Event : public PObject
    {
    public:
        enum class ExecutionPolicy
        {
            Inline = 0, Background
        };

        Event() = default;
        virtual ~Event() = default;

        // Get the type identifier of the event
        virtual EventType getType() const = 0;

        // To be implemented...
        // static constexpr EventTypeID getStaticType();

        // Get the event execution thread
        virtual ExecutionPolicy getExecutionThread() const = 0;

        // Convert the event to a string representation
        virtual std::string toString() const = 0;
    };

    template <typename T>
    concept TypeEvent = std::is_base_of_v<Event, T>;

    // Type alias for event callback function
    using EventCallback = std::function<void(Ref<Event>)>;
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
