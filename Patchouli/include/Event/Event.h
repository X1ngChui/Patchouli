#pragma once

#include "PatchouliPch.h"
#include "Core/Base.h"
#include "Core/PObject.h"
#include "Util/TypeTraits.h"
#include "Util/Reference.h"
#include "Util/ThreadPool.h"
#include <fmt/format.h>

#define PATCHOULI_EVENT_BIT(x) (1 << x)

namespace Patchouli
{
    // Type alias for event type identifier
    using event_type = uint32_t;

    enum class EventType : event_type
    {
        None = 0,

        // Control Events
        Termination = PATCHOULI_EVENT_BIT(1),
        Control = Termination,

        // Application Events
        AppUpdate = PATCHOULI_EVENT_BIT(2),
        AppRender = PATCHOULI_EVENT_BIT(3),
        Application = AppUpdate | AppRender,

        // Keyboard Events
        KeyPressed = PATCHOULI_EVENT_BIT(4),
        KeyReleased = PATCHOULI_EVENT_BIT(5),
        KeyTyped = PATCHOULI_EVENT_BIT(6),
        Keyboard = KeyPressed | KeyReleased | KeyTyped,

        // Mouse Events
        MouseButtonPressed = PATCHOULI_EVENT_BIT(7),
        MouseButtonReleased = PATCHOULI_EVENT_BIT(8),
        MouseMoved = PATCHOULI_EVENT_BIT(9),
        MouseScrolled = PATCHOULI_EVENT_BIT(10),
        Mouse = MouseButtonPressed | MouseButtonReleased | MouseMoved | MouseScrolled,

        // Input Events
        Input = Keyboard | Mouse,

        // Window Events
        WindowClose = PATCHOULI_EVENT_BIT(12),
        WindowResize = PATCHOULI_EVENT_BIT(13),
        Window = WindowClose | WindowResize
    };

    // Abstract base class for events
    class PATCHOULI_API Event : public PObject
    {
    public:
        Event() = default;
        virtual ~Event() = default;

        // Get the type identifier of the event
        virtual EventType getType() const = 0;

        // To be implemented...
        // static constexpr EventTypeID getStaticType();

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
