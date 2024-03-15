#pragma once

#include "Event/Event.h"


namespace Patchouli
{
	class KeyboardEvent : public EventBase<KeyboardEvent>
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


	class KeyPressedEvent : public virtual KeyboardEvent
	{
	public:
		KeyPressedEvent(int keyCode, bool repeated)
			: KeyboardEvent(keyCode), repeated(repeated)
		{
		}
		virtual ~KeyPressedEvent() = default;
			 
		constexpr bool isRepeated() const { return repeated; }

		std::string toString() const override
		{
			return fmt::format("KeyPressedEvent (Keycode: {}, Repeated: {})", keyCode, repeated);
		}
	private:
		bool repeated;
	};

	class KeyReleasedEvent : public virtual KeyboardEvent
	{  
	public:
		KeyReleasedEvent(int keyCode)
			: KeyboardEvent(keyCode)
		{
		}
		virtual ~KeyReleasedEvent() = default;

		std::string toString() const override
		{
			return fmt::format("KeyRelaesedEvent (Keycode: {})", keyCode);
		}
	};

	class KeyTypedEvent : public virtual KeyboardEvent
	{
	public:
		KeyTypedEvent(int keyCode)
			: KeyboardEvent(keyCode)
		{
		}
		virtual ~KeyTypedEvent() = default;

		std::string toString() const override 
		{
			return fmt::format("KeyTypedEvent (Char: {})", (char)keyCode);
		}
	};
}