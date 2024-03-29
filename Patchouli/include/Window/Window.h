#pragma once
#include "Core/PObject.h"
#include "Event/Event.h"
#include "Util/Reference.h"

namespace Patchouli
{
	// Enum representing different windowing APIs
	enum class WindowAPI
	{
		None = 0,   // No window
		GLFW        // GLFW windowing API
	};

	struct WindowCreateInfo
	{
		WindowAPI windowAPI = WindowAPI::GLFW;
		std::string windowTitle = "Patchouli";
		uint32_t windowWidth = 1280;
		uint32_t windowHeight = 720;

		EventCallback windowEventCallback;
	};

	class PATCHOULI_API Window : public PObject
	{
	public:
		Window() = default;
		virtual ~Window() = default;

		virtual const std::string& getTitle() const = 0;
		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;

		virtual void show() = 0;
		virtual void hide() = 0;

		virtual void setEventCallback(const EventCallback& eventCallback) = 0;
		virtual void setEventCallback(const EventCallback&& eventCallback) = 0;

		virtual void onUpdate() = 0;

		virtual void* getNative() const = 0;
		virtual WindowAPI getAPI() const = 0;

		static Ref<Window> create(const WindowCreateInfo& info);
	};
}