#pragma once

#include "Event/Event.h"

namespace Patchouli
{
	class AppUpdateEvent final : public EventBase<AppUpdateEvent>
	{
	public:
		AppUpdateEvent() = default;
		virtual ~AppUpdateEvent() = default;
	};


	class AppRenderEvent final : public EventBase<AppRenderEvent>
	{
	public:
		AppRenderEvent() = default;
		virtual ~AppRenderEvent() = default;
	};
}