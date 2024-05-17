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
		virtual std::size_t toString(char* buffer, std::size_t size) const override 
		{
			auto result = fmt::format_to_n(buffer, size, "KeyPressedEvent (Keycode: {}, Repeated: {})", keyCode, repeated);
			return result.size;
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
		virtual std::size_t toString(char* buffer, std::size_t size) const override
		{
			auto result = fmt::format_to_n(buffer, size, "KeyRelaesedEvent (Keycode: {})", keyCode);
			return result.size;
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
		virtual std::size_t toString(char* buffer, std::size_t size) const override
		{
			auto result = fmt::format_to_n(buffer, size, "KeyTypedEvent (Char: {})", (char)keyCode);
			return result.size;
		}
	};
}