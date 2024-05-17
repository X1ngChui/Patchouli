#pragma once

#include "Core/Base.h"
#include "Core/PObject.h"
#include "Util/Reference.h"
#include <fmt/format.h>

#define PATCHOULI_EVENT_STRING_SIZE 128

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
        virtual std::size_t toString(char* buffer, std::size_t size) const = 0;
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
template <>
struct fmt::formatter<Patchouli::Event, char> : fmt::formatter<std::string_view>
{
    template <typename FormatCtx>
    auto format(const Patchouli::Event& evt, FormatCtx& ctx) const
    {
        std::array<char, PATCHOULI_EVENT_STRING_SIZE> string;
        std::size_t len = evt.toString(string.data(), string.size());
        std::string_view view(string.data(), len);
        return fmt::format_to(ctx.out(), "{}", view);
    }
};


