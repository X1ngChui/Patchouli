#pragma once
#include "Event/Event.h"
#include "Util/Util.h"

namespace Patchouli
{
	// Enum representing different windowing APIs
	enum class WindowAPI
	{
		None = 0,   // No window
		GLFW        // GLFW windowing API
	};

	struct WindowInfo
	{
		WindowAPI windowAPI = WindowAPI::GLFW;
		const char* windowTitle = "Patchouli";
		uint32_t windowWidth = 1280;
		uint32_t windowHeight = 720;
	};

	class PATCHOULI_API Window : public RefBase<Window>
	{
	public:
		Window() = default;
		virtual ~Window() = default;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;

		using EventCallback = std::function<void(Event*)>;
		virtual void setEventCallback(const EventCallback& eventCallback) = 0;

		virtual void onUpdate() = 0;

		virtual void* getNative() const = 0;

		static Ref<Window> create(const WindowInfo& info);
	};
}