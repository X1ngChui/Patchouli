#pragma once

#include "Event/Event.h"

namespace Patchouli
{
	class PATCHOULI_API TerminationEvent : public Event
	{
	public:
		TerminationEvent() = default;
		virtual ~TerminationEvent() = default;

		// Get the type identifier of the event
		virtual EventType getType() const override { return EventType::Termination; }

		static constexpr EventType getStaticType() { return EventType::Termination; }

		// Convert the event to a string representation
		virtual std::size_t toString(char* buffer, std::size_t size) const override
		{
			auto result = fmt::format_to_n(buffer, size, "TerminationEvent");
			return result.size;
		}
	};
}