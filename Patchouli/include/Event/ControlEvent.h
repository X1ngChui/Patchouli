#pragma once

#include "Event/Event.h"

namespace Patchouli
{
	class TerminationEvent : public Event
	{
	public:
		TerminationEvent() = default;
		virtual ~TerminationEvent() = default;

		// Get the type identifier of the event
		virtual EventType getType() const override { return EventType::Termination; }

		constexpr static EventType getStaticType() { return EventType::Termination; }

		// Get the event execution thread
		virtual ExecutionPolicy getExecutionThread() const override
		{
			return Event::ExecutionPolicy::Background;
		}

		// Convert the event to a string representation
		virtual std::string toString() const override 
		{
			return "TerminationEvent"; 
		}
	};

	class FenceEvent : public Event
	{
	public:
		FenceEvent() = default;
		virtual ~FenceEvent() = default;

		// Get the type identifier of the event
		virtual EventType getType() const override { return EventType::Fence; }

		static constexpr EventType getStaticType() { return EventType::Fence; }

		// Get the event execution thread
		virtual ExecutionPolicy getExecutionThread() const { return Event::ExecutionPolicy::Background; }

		// Convert the event to a string representation
		virtual std::string toString() const { return "FenceEvent"; }
	};
}