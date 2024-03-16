#pragma once

#include "Window/Window.h"

namespace Patchouli
{
	class NoneWindow : public Window
	{
	public:
		NoneWindow(const WindowCreateInfo& info) { (void)info; };
		virtual ~NoneWindow() = default;

		virtual const std::string& getTitle() const override { static std::string none = "None"; return none; }
		virtual uint32_t getWidth() const override { return 0; }
		virtual uint32_t getHeight() const override { return 0; }

		virtual void show() override {}
		virtual void hide() override {}

		virtual void setEventCallback(const EventCallback& eventCallback) override { (void)eventCallback; }
		virtual void setEventCallback(const EventCallback&& eventCallback) override { (void)eventCallback; }

		virtual void onUpdate() override {}

		virtual void* getNative() const override { return nullptr; };

		virtual WindowAPI getAPI() const override { return WindowAPI::None; }
	};
}