#pragma once

#include "Window/Window.h"
#include <GLFW/glfw3.h>
#undef APIENTRY

namespace Patchouli
{
	class GLFWWindow : public Window
	{
	public:
		GLFWWindow(const WindowInfo& info);
		virtual ~GLFWWindow();

		virtual uint32_t getWidth() const override { return attribute.width; }
		virtual uint32_t getHeight() const override { return attribute.height; }

		virtual void show() override;
		virtual void hide() override;

		virtual void setEventCallback(const EventCallback& eventCallback) override
		{
			attribute.eventCallback = eventCallback;
		}

		virtual void setEventCallback(const EventCallback&& eventCallback) override
		{
			attribute.eventCallback = eventCallback;
		}

		virtual void onUpdate() override;

		virtual void* getNative() const override { return (void*)(&window); };

	private:
		struct WindowAttribute
		{
			const char* name;
			uint32_t width;
			uint32_t height;

			EventCallback eventCallback;
		};

		GLFWwindow* window;
		WindowAttribute attribute;

		static inline bool glfwInitialized = false;
	};
}