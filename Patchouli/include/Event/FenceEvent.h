#pragma once

#include "Event/Event.h"

namespace Patchouli
{
	class FenceEvent final : public EventBase<FenceEvent, Event::ExecutionThread::Main>
	{
	public:
		FenceEvent() = default;
		virtual ~FenceEvent() = default;
	};
}