#pragma once

#include "Event/Event.h"

namespace Patchouli
{
	class AppTickEvent : public EventBase<AppTickEvent>
	{
	public:
		AppTickEvent() = default;
		virtual ~AppTickEvent() = default;
	};


	class AppUpdateEvent : public EventBase<AppUpdateEvent>
	{
	public:
		AppUpdateEvent() = default;
		virtual ~AppUpdateEvent() = default;
	};


	class AppRenderEvent : public EventBase<AppRenderEvent>
	{
	public:
		AppRenderEvent() = default;
		virtual ~AppRenderEvent() = default;
	};
}