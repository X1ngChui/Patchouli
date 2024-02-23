#pragma once

#include "PatchouliPch.h"
#include "Core/Base.h"
#include "Util/Util.h"
#include "fmt/format.h"

#define PATCHOULI_EVENTCATEGORY_FLAGS(x) (1 << x)

namespace Patchouli
{
	// Enumeration defining different types of events
	enum class EventType
	{
		None = 0,
		WindowClosed, WindowMoved, WindowResize, WindowFocus, WindowLostFucous,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	// Bitmask-based enumeration defining event categories
	enum EventCategory
	{
		None = 0,
		EventCategoryApp = PATCHOULI_EVENTCATEGORY_FLAGS(0),
		EventCategoryWindow = PATCHOULI_EVENTCATEGORY_FLAGS(1),
		EventCategoryInput = PATCHOULI_EVENTCATEGORY_FLAGS(2),
		EventCategoryKeyboard = PATCHOULI_EVENTCATEGORY_FLAGS(3),
		EventCategoryMouse = PATCHOULI_EVENTCATEGORY_FLAGS(4),
		EventCategoryMouseButton = PATCHOULI_EVENTCATEGORY_FLAGS(5)
	};

	// Abstract base class for events
	class PATCHOULI_API Event : public FastAllocated
	{
	public:
		Event() = default;
		virtual ~Event() = default;

		virtual EventType getEventType() const = 0;
		virtual int getCategoryFlag() const = 0;

		virtual std::string toString() const = 0;

		constexpr bool belongToCategory(EventCategory category) const { return getCategoryFlag() & category; }
	private:
		friend class EventDispatcher;
		bool dealt = false;
	};

	// Class responsible for dispatching events and handling them
	class EventDispatcher
	{
	public:
		// Constructor taking an event reference to be dispatched
		EventDispatcher(Event& evt)
			: event(evt)
		{
		}

		bool isDealt() { return event.dealt; }

		// Dispatch an event to a handler function
		template<typename E, typename = std::enable_if_t<std::is_base_of_v<Event, E>>>
		bool dispatch(std::function<bool(E&)> handler)
		{
			if (event.getEventType() == E::getStaticType())
			{
				event.dealt |= handler(static_cast<E&>(event));
				return true;
			}
			return false;
		}
	private:
		Event& event;
	};
}

// Specialization of the formatter for Event types in fmt library
template <typename T>
struct fmt::formatter<T, std::enable_if_t<std::is_base_of_v<Patchouli::Event, T>, char>>
	: fmt::formatter<std::string>
{
	template <typename FormatCtx>
	auto format(const Patchouli::Event& evt, FormatCtx& ctx) const
	{
		return fmt::formatter<std::string>::format(evt.toString(), ctx);
	}
};
