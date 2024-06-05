#pragma once

#include "Event/Event.h"

namespace Patchouli
{
	class PATCHOULI_API MouseButtonEvent : public Event
	{
	public:
		virtual ~MouseButtonEvent() = default;

		inline int getMouseButton() const { return button; }

	protected:
		int button;

		MouseButtonEvent(int button)
			: button(button)
		{
		}
	};


	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button)
		{
		}
		virtual ~MouseButtonPressedEvent() = default;

		// Get the type identifier of the event
		virtual EventType getType() const override { return EventType::MouseButtonPressed; }

		static constexpr EventType getStaticType() { return EventType::MouseButtonPressed; }

		// Convert the event to a string representation
		virtual std::size_t toString(char* buffer, std::size_t size) const override
		{
			auto result = fmt::format_to_n(buffer, size, "MouseButtonPressedEvent (Button: {})", button);
			return result.size;
		}
	};


	class PATCHOULI_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button)
		{
		}
		virtual ~MouseButtonReleasedEvent() = default;

		// Get the type identifier of the event
		virtual EventType getType() const override { return EventType::MouseButtonReleased; }

		static constexpr EventType getStaticType() { return EventType::MouseButtonReleased; }

		// Convert the event to a string representation
		virtual std::size_t toString(char* buffer, std::size_t size) const override
		{
			auto result = fmt::format_to_n(buffer, size, "MouseButtonReleaseEvent (Button: {})", button);
			return result.size;
		}
	};
	
	class PATCHOULI_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y)
			: posX(x), posY(y)
		{
		}
		virtual ~MouseMovedEvent() = default;

		float getPosX() const { return posX; }
		float getPosY() const { return posY; }
		std::pair<float, float> getPos() const { return { posX, posY }; }

		// Get the type identifier of the event
		virtual EventType getType() const override { return EventType::MouseMoved; }

		static constexpr EventType getStaticType() { return EventType::MouseMoved; }

		// Convert the event to a string representation
		virtual std::size_t toString(char* buffer, std::size_t size) const override
		{
			auto result = fmt::format_to_n(buffer, size, "MouseMovedEvent (Pos: ({}, {}))", posX, posY);
			return result.size;
		}

	private:
		float posX, posY;
	};

	class PATCHOULI_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(std::pair<float, float> pos)
			: pos(pos)
		{
		}

		MouseScrolledEvent(float offsetX, float offsetY)
			: pos({ offsetX, offsetY })
		{
		}

		virtual ~MouseScrolledEvent() = default;

		float getOffsetX() const { return pos.first; }
		float getOffsetY() const { return pos.second; }
		std::pair<float, float> getOffset() const { return pos; }

		// Get the type identifier of the event
		virtual EventType getType() const override { return EventType::MouseScrolled; }

		static constexpr EventType getStaticType() { return EventType::MouseScrolled; }

		// Convert the event to a string representation
		virtual std::size_t toString(char* buffer, std::size_t size) const override
		{
			auto result = fmt::format_to_n(buffer, size, "MouseScrolledEvent (Offset: ({}, {}))", pos.first, pos.second);
			return result.size;
		}

	private:
		std::pair<float, float> pos;
	};
}