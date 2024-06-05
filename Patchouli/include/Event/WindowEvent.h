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
		WindowResizeEvent(std::pair<unsigned int, unsigned int>)
			: size(size)
		{
		}

		WindowResizeEvent(unsigned int width, unsigned int height)
			: size({ width, height })
		{
		}
		virtual ~WindowResizeEvent() = default;
		
		std::pair<unsigned int, unsigned int> getSize() const { return size; }
		unsigned int getWidth() const { return size.first; }
		unsigned int getHeight() const { return size.second; }

		// Get the type identifier of the event
		virtual EventType getType() const override { return EventType::WindowResize; }

		static constexpr EventType getStaticType() { return EventType::WindowResize; }

		// Convert the event to a string representation
		virtual std::size_t toString(char* buffer, std::size_t size) const override
		{
			auto result = fmt::format_to_n(buffer, size, "WindowResizeEvent (Size: ({}, {}))", 
				this->size.first, this->size.second);
			return result.size;
		}

	private:
		std::pair<unsigned int, unsigned int> size;
	};
}

