#pragma once

#include "Event/Event.h"

namespace Patchouli
{
	class PATCHOULI_API AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;
		virtual ~AppUpdateEvent() = default;

		// Get the type identifier of the event
		virtual EventType getType() const override { return EventType::AppUpdate; }

		static constexpr EventType getStaticType() { return EventType::AppUpdate; }

		// Convert the event to a string representation
		virtual std::size_t toString(char* buffer, std::size_t size) const override {
			auto result = fmt::format_to_n(buffer, size, "AppUpdateEvent");
			return result.size;
		}
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
		virtual std::size_t toString(char* buffer, std::size_t size) const override {
			auto result = fmt::format_to_n(buffer, size, "AppRenderEvent");
			return result.size;
		}
	};
}