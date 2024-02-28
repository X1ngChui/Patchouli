#pragma once

#include "Event/Event.h"

namespace Patchouli
{
	class MouseButtonEvent : public Event
	{
	public:
		virtual ~MouseButtonEvent() = default;

		inline int getMouseButton() const { return button; }

		virtual constexpr int getCategoryFlag() const override
		{
			return EventCategory::EventCategoryInput | EventCategory::EventCategoryMouse | EventCategory::EventCategoryMouseButton;
		}
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

		static constexpr EventType getStaticType() { return EventType::MouseButtonPressed; }
		virtual constexpr EventType getEventType() const override { return EventType::MouseButtonPressed; }

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

		static constexpr EventType getStaticType() { return EventType::MouseButtonReleased; }
		virtual constexpr EventType getEventType() const override { return EventType::MouseButtonReleased; }

		std::string toString() const override
		{
			return fmt::format("MouseButtonRelease (Button: {})", button);
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

		virtual constexpr int getCategoryFlag() const override
		{
			return EventCategory::EventCategoryInput | EventCategory::EventCategoryMouse;
		}

		static constexpr EventType getStaticType() { return EventType::MouseMoved; }
		virtual constexpr EventType getEventType() const override { return EventType::MouseMoved; }

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

		virtual constexpr int getCategoryFlag() const override 
		{
			return EventCategory::EventCategoryInput | EventCategory::EventCategoryMouse;
		}

		static constexpr EventType getStaticType() { return EventType::MouseScrolled; }
		virtual constexpr EventType getEventType() const override { return EventType::MouseScrolled; }

		std::string toString() const override
		{
			return fmt::format("MouseScrolledEvent (Offset: ({}, {}))", offsetX, offsetY);
		}

	private:
		float offsetX, offsetY;
	};
}