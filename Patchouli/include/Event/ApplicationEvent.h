#pragma once

#include "Event/Event.h"

namespace Patchouli
{
	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;
		virtual ~AppUpdateEvent() = default;

		// Get the type identifier of the event
		virtual EventType getType() const override { return EventType::AppUpdate; }

		static EventType getStaticType() { return EventType::AppUpdate; }

		// Get the event execution thread
		virtual ExecutionPolicy getExecutionThread() const override 
		{
			return Event::ExecutionPolicy::Background;
		}

		// Convert the event to a string representation
		virtual std::string toString() const override { return "AppUpdateEvent"; }
	};


	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;
		virtual ~AppRenderEvent() = default;

		// Get the type identifier of the event
		virtual EventType getType() const override { return EventType::AppRender; }

		static constexpr EventType getStaticType() { return EventType::AppRender; }

		// Get the event execution thread
		virtual ExecutionPolicy getExecutionThread() const override
		{
			return Event::ExecutionPolicy::Background;
		};

		// Convert the event to a string representation
		virtual std::string toString() const { return "AppRenderEvent"; }

		
	};
}