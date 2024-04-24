#pragma once

#include "Event/Event.h"

#define PATCHOULI_EVENT_TOPIC_APP Patchouli::AppUpdateEvent, Patchouli::AppRenderEvent

namespace Patchouli
{
	class PATCHOULI_API AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;
		virtual ~AppUpdateEvent() = default;

		// Get the type identifier of the event
		virtual EventType getType() const override { return EventType::AppUpdate; }

		static EventType getStaticType() { return EventType::AppUpdate; }

		// Convert the event to a string representation
		virtual std::string toString() const override { return "AppUpdateEvent"; }
	};


	class PATCHOULI_API AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;
		virtual ~AppRenderEvent() = default;

		// Get the type identifier of the event
		virtual EventType getType() const override { return EventType::AppRender; }

		static constexpr EventType getStaticType() { return EventType::AppRender; }

		// Convert the event to a string representation
		virtual std::string toString() const { return "AppRenderEvent"; }
	};
}