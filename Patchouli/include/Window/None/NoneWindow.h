#pragma once

#include "Window/Window.h"

namespace Patchouli
{
	class NoneWindow : public Window
	{
	public:
		NoneWindow(const WindowInfo& info) { (void)info; };
		virtual ~NoneWindow() = default;

		virtual uint32_t getWidth() const override { return 0; }
		virtual uint32_t getHeight() const override { return 0; }

		virtual void show() override {}
		virtual void hide() override {}

		virtual void setEventCallback(const EventCallback& eventCallback) override { (void)eventCallback; }

		virtual void onUpdate() override {}

		virtual void* getNative() const override { return nullptr; };
	};
}