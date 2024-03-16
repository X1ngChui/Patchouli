#pragma once

#include "Window/Window.h"
#include <GLFW/glfw3.h>
#undef APIENTRY

namespace Patchouli
{
	class GLFWWindow : public Window
	{
	public:
		GLFWWindow(const WindowCreateInfo& info);
		virtual ~GLFWWindow();
		
		virtual const std::string& getTitle() const override { return attribute.title; }
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

		virtual WindowAPI getAPI() const override { return WindowAPI::GLFW; }

	private:
		struct WindowAttribute
		{
			std::string title;
			uint32_t width;
			uint32_t height;

			EventCallback eventCallback;
		};

		GLFWwindow* window;
		WindowAttribute attribute;

		static inline bool glfwInitialized = false;
	};
}