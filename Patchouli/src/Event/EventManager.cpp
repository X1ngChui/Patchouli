#include "Event/EventManager.h"
#include "Event/ControlEvent.h"
#include "Event/EventHandler.h"

namespace Patchouli
{
	// Implementation of adding an event listener
	void EventManager::addHandlerImpl(Ref<EventHandlerBase> handler, EventType type)
	{
		std::unique_lock<std::mutex> lock(mapMutex);
		handlerMap.insert({ type, handler });
	}

	// Implementation of removing an event listener
	void EventManager::removeHandlerImpl(Ref<EventHandlerBase> handler, EventType type)
	{
		std::unique_lock<std::mutex> lock(mapMutex);
		auto range = handlerMap.equal_range(type);

		for (auto& it = range.first; it != range.second; ++it) {
			if (it->second == handler) 
			{
				handlerMap.erase(it);
				break;
			}
		}
	}

	// Method to publish an event to the event queue
	EventManager& EventManager::publish(Ref<Event> event)
	{
		std::lock_guard<std::mutex> lock(queueMutex);
		eventQueue.push(event);
		return *this;
	}

	// Method to start the event loop
	void EventManager::run()
	{
		running = true;

		while (running)
		{
			Ref<Event> event;
			{
				std::unique_lock<std::mutex> lock(queueMutex);
				loopCv.wait(lock, [this] { return !eventQueue.empty(); });
				event = eventQueue.front();
				eventQueue.pop();
			}

			onEvent(event);
		}
	}

	void EventManager::onEvent(Ref<Event> event)
	{
		EventType type = event->getType();

		// Special Event for event loop control
		switch (type)
		{
		case TerminationEvent::getStaticType():
			onTerminationEvent();
			break;
		}

		// Process each event handlers
		std::unique_lock<std::mutex> lock(mapMutex);
		auto range = handlerMap.equal_range(type);

		for (auto& it = range.first; it != range.second; it++)
		{
			Ref<EventHandlerBase>& handler = it->second;
			(*handler)(event);
		}
	}

	void EventManager::onTerminationEvent()
	{
		running = false;
	}
}