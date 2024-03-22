#pragma once
#include "Event/Event.h"

namespace Patchouli
{
	class TerminationEvent final : public EventBase<TerminationEvent>
	{
	public:
		TerminationEvent() = default;
		virtual ~TerminationEvent() = default;
	};
}