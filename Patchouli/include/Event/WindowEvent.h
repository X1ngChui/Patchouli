#pragma once

#include "Event/Event.h"

namespace Patchouli
{
	class PATCHOULI_API WindowCloseEvent final : public Event
	{
	public:
		WindowCloseEvent() = default;
		virtual ~WindowCloseEvent() = default;

		// Get the type identifier of the event
		virtual EventType getType() const override { return EventType::WindowClose; }

		static constexpr EventType getStaticType() { return EventType::WindowClose; }

		// Convert the event to a string representation
		std::string toString() const override
		{
			return "WindowCloseEvent";
		}
	};

	class PATCHOULI_API WindowResizeEvent final : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: windowWidth(width), windowHeight(height)
		{
		}
		virtual ~WindowResizeEvent() = default;
		
		unsigned int getWidth() const { return windowWidth; }
		unsigned int getHeight() const { return windowHeight; }

		// Get the type identifier of the event
		virtual EventType getType() const override { return EventType::WindowResize; }

		static constexpr EventType getStaticType() { return EventType::WindowResize; }

		// Convert the event to a string representation
		std::string toString() const override
		{
			return fmt::format("WindowResizeEvent (Size: ({}, {}))", windowWidth, windowHeight);
		}

	private:
		unsigned int windowWidth, windowHeight;
	};
}

