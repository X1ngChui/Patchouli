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
        // static constexpr EventType getStaticType();

        // Convert the event to a string representation
        virtual std::string toString() const = 0;
    };

    /*
     * Concept TypeEvent checks if a type meets the requirements to be considered as an Event type.
     * A type T satisfies TypeEvent if it:
     * - Is derived from the base class Event.
     * - Is not an abstract class.
     * - Provides a static member function getStaticType() that returns a constexpr EventType.
     */
    template <typename T>
    concept TypeEvent = std::is_base_of_v<Event, T> && !std::is_abstract_v<T> && requires {
        { T::getStaticType() } -> std::same_as<EventType>;
        { std::integral_constant<EventType, T::getStaticType()>{} };
    };

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
