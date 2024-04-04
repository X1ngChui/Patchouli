#pragma once

#include "Event/Event.h"


namespace Patchouli
{
	class PATCHOULI_API KeyboardEvent : public Event
	{
	public:
		virtual ~KeyboardEvent() = default;

		int getKeyCode() const { return keyCode; }

	protected:
		int keyCode;
		
		KeyboardEvent(int keyCode)
			: keyCode(keyCode)
		{
		}
	};

	class PATCHOULI_API KeyPressedEvent : public KeyboardEvent
	{
	public:
		KeyPressedEvent(int keyCode, bool repeated)
			: KeyboardEvent(keyCode), repeated(repeated)
		{
		}
		virtual ~KeyPressedEvent() = default;
			 
		constexpr bool isRepeated() const { return repeated; }

		// Get the type identifier of the event
		virtual EventType getType() const override { return EventType::KeyPressed; }

		static constexpr EventType getStaticType() { return EventType::KeyPressed; }

		// Convert the event to a string representation
		std::string toString() const override
		{
			return fmt::format("KeyPressedEvent (Keycode: {}, Repeated: {})", keyCode, repeated);
		}

	private:
		bool repeated;
	};

	class PATCHOULI_API KeyReleasedEvent : public KeyboardEvent
	{  
	public:
		KeyReleasedEvent(int keyCode)
			: KeyboardEvent(keyCode)
		{
		}
		virtual ~KeyReleasedEvent() = default;

		// Get the type identifier of the event
		virtual EventType getType() const override { return EventType::KeyReleased; }

		static constexpr EventType getStaticType() { return EventType::KeyReleased; }

		// Convert the event to a string representation
		std::string toString() const override
		{
			return fmt::format("KeyRelaesedEvent (Keycode: {})", keyCode);
		}
	};

	class PATCHOULI_API KeyTypedEvent : public KeyboardEvent
	{
	public:
		KeyTypedEvent(int keyCode)
			: KeyboardEvent(keyCode)
		{
		}
		virtual ~KeyTypedEvent() = default;

		// Get the type identifier of the event
		virtual EventType getType() const override { return EventType::KeyTyped; }

		static constexpr EventType getStaticType() { return EventType::KeyTyped; }

		// Convert the event to a string representation
		std::string toString() const override 
		{
			return fmt::format("KeyTypedEvent (Char: {})", (char)keyCode);
		}
	};
}