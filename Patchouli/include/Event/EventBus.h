#pragma once

#include "Core/Base.h"
#include "Event/Event.h"
#include "PatchouliPch.h"

namespace Patchouli
{
	class PATCHOULI_API EventBus
	{
	public:
		EventBus() = default;
		~EventBus() = default;

		void push(Ref<Event> event);

		void push(std::initializer_list<Ref<Event>> events);

		Ref<Event> pop();

	private:
		std::mutex mutex;
		std::condition_variable cv;
		std::queue<Ref<Event>> eventQueue;
	};
}