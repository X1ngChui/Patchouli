#pragma once

#include "Event/Event.h"


namespace Patchouli
{
	class KeyboardEvent : public Event
	{
	public:
		virtual ~KeyboardEvent() = default;

		int getKeyCode() const { return keyCode; }

		virtual constexpr int getCategoryFlag() const override
		{
			return EventCategory::EventCategoryInput | EventCategory::EventCategoryKeyboard;
		}

	protected:
		int keyCode;
		
		KeyboardEvent(int keyCode)
			: keyCode(keyCode)
		{
		}
	};


	class KeyPressedEvent : public KeyboardEvent
	{
	public:
		KeyPressedEvent(int keyCode, bool repeated)
			: KeyboardEvent(keyCode), repeated(repeated)
		{
		}
		virtual ~KeyPressedEvent() = default;
			 
		constexpr bool isRepeated() const { return repeated; }

		static constexpr EventType getStaticType() { return EventType::KeyPressed; }
		virtual constexpr EventType getEventType() const override { return EventType::KeyPressed; }

		std::string toString() const override
		{
			return fmt::format("KeyPressedEvent (Keycode: {}, Repeated: {})", keyCode, repeated);
		}
	private:
		bool repeated;
	};

	class KeyReleasedEvent : public KeyboardEvent
	{  
	public:
		KeyReleasedEvent(int keyCode)
			: KeyboardEvent(keyCode)
		{
		}
		virtual ~KeyReleasedEvent() = default;

		static constexpr EventType getStaticType() { return EventType::KeyReleased; }
		virtual constexpr EventType getEventType() const override { return EventType::KeyReleased; }

		std::string toString() const override
		{
			return fmt::format("KeyRelaesedEvent (Keycode: {})", keyCode);
		}
	};

	class KeyTypedEvent : public KeyboardEvent
	{
	public:
		KeyTypedEvent(int keyCode)
			: KeyboardEvent(keyCode)
		{
		}
		virtual ~KeyTypedEvent() = default;

		static constexpr EventType getStaticType() { return EventType::KeyTyped; }
		virtual constexpr EventType getEventType() const override { return EventType::KeyTyped; }

		std::string toString() const override 
		{
			return fmt::format("KeyTypedEvent (Char: {})", (char)keyCode);
		}
	};
}