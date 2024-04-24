#pragma once

#include "Event/Event.h"

#define PATCHOULI_EVENT_TOPIC_CONTROL Patchouli::TerminationEvent

namespace Patchouli
{
	class PATCHOULI_API TerminationEvent : public Event
	{
	public:
		TerminationEvent() = default;
		virtual ~TerminationEvent() = default;

		// Get the type identifier of the event
		virtual EventType getType() const override { return EventType::Termination; }

		constexpr static EventType getStaticType() { return EventType::Termination; }

		// Convert the event to a string representation
		virtual std::string toString() const override 
		{
			return "TerminationEvent"; 
		}
	};
}