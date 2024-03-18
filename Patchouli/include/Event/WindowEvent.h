#pragma once

#include "Event/Event.h"

namespace Patchouli
{
	class WindowUpdateEvent final : public EventBase<WindowUpdateEvent, Event::ExecutionThread::Main>
	{
	public:
		WindowUpdateEvent() = default;

		virtual ~WindowUpdateEvent() = default;
	};

	class WindowCloseEvent final : public EventBase<WindowCloseEvent>
	{
	public:
		WindowCloseEvent() = default;
		virtual ~WindowCloseEvent() = default;
	};


	class WindowResizeEvent final : public EventBase<WindowResizeEvent>
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: windowWidth(width), windowHeight(height)
		{
		}
		virtual ~WindowResizeEvent() = default;
		
		unsigned int getWidth() const { return windowWidth; }
		unsigned int getHeight() const { return windowHeight; }

		std::string toString() const override 
		{
			return fmt::format("WindowResizeEvent (Size: ({}, {}))", windowWidth, windowHeight);
		}

	private:
		unsigned int windowWidth, windowHeight;
	};
}

