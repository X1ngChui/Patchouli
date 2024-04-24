#pragma once

#include "PatchouliPch.h"
#include "Core/Base.h"
#include "Core/PObject.h"
#include "Util/TypeTraits.h"
#include "Util/Reference.h"
#include <fmt/format.h>

namespace Patchouli
{
    // Type alias for event type identifier
    using EventID = uint64_t;

    enum class EventType : EventID
    {
        None = 0,

        // Control Events
        Termination,

        // Application Events
        AppUpdate,
        AppRender,

        // Keyboard Events
        KeyPressed,
        KeyReleased,
        KeyTyped,
        Keyboard,

        // Mouse Events
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled,

        // Window Events
        WindowClose,
        WindowResize
    };

    // Abstract base class for events
    class PATCHOULI_API Event : public PObject
    {
    public:
        Event() = default;
        virtual ~Event() = default;

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
