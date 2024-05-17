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
		virtual std::size_t toString(char* buffer, std::size_t size) const override
		{
			auto result = fmt::format_to_n(buffer, size, "WindowCloseEvent");
			return result.size;
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
		virtual std::size_t toString(char* buffer, std::size_t size) const override
		{
			auto result = fmt::format_to_n(buffer, size, "WindowResizeEvent (Size: ({}, {}))", windowWidth, windowHeight);
			return result.size;
		}

	private:
		unsigned int windowWidth, windowHeight;
	};
}

