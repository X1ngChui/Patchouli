#include "Event/EventBus.h"

namespace Patchouli
{
	void EventBus::push(Ref<Event> event)
	{
		std::unique_lock<std::mutex> lock(mutex);
		eventQueue.push(event);
		cv.notify_all();
	}

	void EventBus::push(std::initializer_list<Ref<Event>> events)
	{
		std::unique_lock<std::mutex> lock(mutex);
		for (auto& event : events)
			eventQueue.push(event);
		cv.notify_all();
	}

	Ref<Event> EventBus::pop()
	{
		std::unique_lock<std::mutex> lock(mutex);
		cv.wait(lock, [this] { return !eventQueue.empty(); });
		Ref<Event> event = eventQueue.front();
		eventQueue.pop();
		return event;
	}
}
