#pragma once

#include "Event/Event.h"

namespace Patchouli
{
	class MouseButtonEvent : public Event
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

		// Get the event execution thread
		virtual ExecutionPolicy getExecutionThread() const override
		{
			return Event::ExecutionPolicy::Background;
		}

		// Convert the event to a string representation
		std::string toString() const override
		{
			return fmt::format("MouseButtonPressedEvent (Button: {})", button);
		}
	};


	class MouseButtonReleasedEvent : public MouseButtonEvent
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

		// Get the event execution thread
		virtual ExecutionPolicy getExecutionThread() const override
		{
			return Event::ExecutionPolicy::Background;
		}

		// Convert the event to a string representation
		std::string toString() const override
		{
			return fmt::format("MouseButtonReleaseEvent (Button: {})", button);
		}
	};
	
	class MouseMovedEvent : public Event
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

		// Get the event execution thread
		virtual ExecutionPolicy getExecutionThread() const override
		{
			return Event::ExecutionPolicy::Background;
		}

		// Convert the event to a string representation
		std::string toString() const override
		{
			return fmt::format("MouseMovedEvent (Pos: ({}, {}))", posX, posY);
		}

	private:
		float posX, posY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float offsetX, float offsetY)
			: offsetX(offsetX), offsetY(offsetY)
		{
		}
		virtual ~MouseScrolledEvent() = default;

		float getOffsetX() const { return offsetX; }
		float getOffsetY() const { return offsetY; }
		std::pair<float, float> getOffset() const { return { offsetX, offsetY }; }

		// Get the type identifier of the event
		virtual EventType getType() const override { return EventType::MouseScrolled; }

		static constexpr EventType getStaticType() { return EventType::MouseScrolled; }

		// Get the event execution thread
		virtual ExecutionPolicy getExecutionThread() const override
		{
			return Event::ExecutionPolicy::Background;
		}

		// Convert the event to a string representation
		std::string toString() const override
		{
			return fmt::format("MouseScrolledEvent (Offset: ({}, {}))", offsetX, offsetY);
		}

	private:
		float offsetX, offsetY;
	};
}