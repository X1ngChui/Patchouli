#pragma once

#include "Event/Event.h"

namespace Patchouli
{
	class MouseButtonEvent
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


	class MouseButtonPressedEvent final : public MouseButtonEvent, public EventBase<MouseButtonPressedEvent>
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button)
		{
		}
		virtual ~MouseButtonPressedEvent() = default;

		std::string toString() const override
		{
			return fmt::format("MouseButtonPressedEvent (Button: {})", button);
		}
	};


	class MouseButtonReleasedEvent final : public MouseButtonEvent, public EventBase<MouseButtonReleasedEvent>
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button)
		{
		}
		virtual ~MouseButtonReleasedEvent() = default;

		std::string toString() const override
		{
			return fmt::format("MouseButtonRelease (Button: {})", button);
		}
	};
	
	class MouseMovedEvent final : public EventBase<MouseMovedEvent>
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

		std::string toString() const override 
		{
			return fmt::format("MouseMovedEvent (Pos: ({}, {}))", posX, posY);
		}
	private:
		float posX, posY;
	};

	class MouseScrolledEvent final : public EventBase<MouseScrolledEvent>
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

		std::string toString() const override
		{
			return fmt::format("MouseScrolledEvent (Offset: ({}, {}))", offsetX, offsetY);
		}

	private:
		float offsetX, offsetY;
	};
}