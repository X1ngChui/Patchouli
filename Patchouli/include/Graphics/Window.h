#pragma once
#include "Event/Event.h"
#include "Util/Util.h"

namespace Patchouli
{
	struct WindowInfo
	{
		const char* title;
		uint32_t width;
		uint32_t height;
	};

	class PATCHOULI_API Window : public RefCounted<Window>
	{
	public:
		Window() = default;
		virtual ~Window() = default;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;

		using EventCallback = std::function<void(Event*)>;
		virtual void setEventCallback(const EventCallback& eventCallback) = 0;

		virtual void onUpdate() = 0;

		static Ref<Window> create(const WindowInfo& info);
	};
}