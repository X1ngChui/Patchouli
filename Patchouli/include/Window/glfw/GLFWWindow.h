#pragma once

#include "Window/Window.h"
#include "Window/glfw/GLFWProxy.h"

#define PATCHOULI_WINDOW_TITLE_SIZE 256

namespace Patchouli
{
	class GLFWWindow : public Window
	{
	public:
		GLFWWindow(const WindowCreateInfo& info);
		virtual ~GLFWWindow();
		
		virtual const char* getTitle() const override { return attribute.title; }
		virtual glm::vec2 getSize() const override { return attribute.size; }
		virtual uint32_t getWidth() const override { return attribute.size.x; }
		virtual uint32_t getHeight() const override { return attribute.size.y; }

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
			char title[PATCHOULI_WINDOW_TITLE_SIZE];
			glm::vec2 size;

			EventCallback eventCallback;
		};

		GLFWwindow* window;
		WindowAttribute attribute;
	};
}