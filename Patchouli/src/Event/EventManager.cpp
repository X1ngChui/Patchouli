#include "Event/EventManager.h"
#include "Event/ControlEvent.h"
#include "Event/EventHandler.h"

#define PATCHOULI_EVENT_BUFFER_SIZE 8

namespace Patchouli
{
	// Implementation of adding an event listener
	void EventManager::addHandler(Ref<EventHandler> handler, EventType type)
	{
		std::unique_lock<std::mutex> lock(mapMutex);

		event_type types = (event_type)type;
		while (types)
		{
			event_type tp = types & (-types);
			types &= ~tp;

			handlerMap.insert({ (EventType)tp, handler });
		}
		
	}

	// Implementation of removing an event listener
	void EventManager::removeHandler(Ref<EventHandler> handler)
	{
		std::unique_lock<std::mutex> lock(mapMutex);
		auto it = handlerMap.begin();
		while (it != handlerMap.end())
		{
			if (it->second == handler)
				it = handlerMap.erase(it);
			else
				++it;
		}
	}

	// Method to publish an event to the event queue
	void EventManager::publishEvent(Ref<Event> event)
	{
		eventQueue.enqueue(event);
	}

	// Method to start the event loop
	void EventManager::run()
	{
		running = true;

		// Buffer to hold events to process
		std::array<Ref<Event>, PATCHOULI_EVENT_BUFFER_SIZE> eventBuffer;

		while (running)
		{
			// Dequeue several events from the event queue
			std::size_t nEvents = eventQueue.wait_dequeue_bulk(eventBuffer.data(), eventBuffer.size());

			// Process each event
			for (std::size_t i = 0; i < nEvents; i++)
				onEvent(eventBuffer[i]);
		}
	}

	void EventManager::onEvent(Ref<Event>& event)
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

		for (auto it = range.first; it != range.second; it++)
		{
			Ref<EventHandler>& handler = it->second;
			(*handler)(event);
		}
	}

	void EventManager::onTerminationEvent()
	{
		running = false;
	}
}