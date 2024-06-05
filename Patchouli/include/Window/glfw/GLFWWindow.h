#pragma once

#include "Window/Window.h"
#include "Window/glfw/GLFWProxy.h"

namespace Patchouli
{
	constexpr std::size_t windowTitleSize = 256;

	class GLFWWindow : public Window
	{
	public:
		GLFWWindow(const WindowCreateInfo& info);
		virtual ~GLFWWindow();
		
		virtual const char* getTitle() const override { return attribute.title; }
		virtual std::pair<uint32_t, uint32_t> getSize() const override { return attribute.size; }
		virtual uint32_t getWidth() const override { return attribute.size.first; }
		virtual uint32_t getHeight() const override { return attribute.size.second; }

		virtual void show() override;
		virtual void hide() override;

		virtual void setEventCallback(const EventCallback& eventCallback) override
		{
			attribute.eventCallback = eventCallback;
		}

		virtual void setEventCallback(EventCallback&& eventCallback) override
		{
			attribute.eventCallback = std::move(eventCallback);
		}

		virtual void onUpdate() override;

		using NativeData = GLFWwindow**;
		virtual void* getNativeData() const override { return (void*)(&window); };

		virtual WindowAPI getAPI() const override { return WindowAPI::GLFW; }

	private:
		struct WindowAttribute
		{
			char title[windowTitleSize];
			std::pair<uint32_t, uint32_t> size;

			EventCallback eventCallback;
		};

		GLFWwindow* window;
		WindowAttribute attribute;
	};
}