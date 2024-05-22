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
		WindowResizeEvent(glm::uvec2 size)
			: size(size)
		{
		}

		WindowResizeEvent(unsigned int width, unsigned int height)
			: size({ width, height })
		{
		}
		virtual ~WindowResizeEvent() = default;
		
		glm::uvec2 getSize() const { return size; }
		unsigned int getWidth() const { return size.x; }
		unsigned int getHeight() const { return size.y; }

		// Get the type identifier of the event
		virtual EventType getType() const override { return EventType::WindowResize; }

		static constexpr EventType getStaticType() { return EventType::WindowResize; }

		// Convert the event to a string representation
		virtual std::size_t toString(char* buffer, std::size_t size) const override
		{
			auto result = fmt::format_to_n(buffer, size, "WindowResizeEvent (Size: ({}, {}))", 
				this->size.x, this->size.y);
			return result.size;
		}

	private:
		glm::uvec2 size;
	};
}

