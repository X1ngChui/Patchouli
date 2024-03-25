#include "Event/EventDispatecher.h"
#include "Event/ControlEvent.h"
#include "Event/EventListener.h"

#define PATCHOULI_EVENT_BUFFER_SIZE 16

namespace Patchouli
{
	// Implementation of adding an event listener
	void EventDispatcher::addListener(Ref<EventListenerBase> listener)
	{
		std::unique_lock<std::mutex> lock(mapMutex);
		listenerMap.insert({ listener->getType(), listener });

		listener->dispatcher = std::static_pointer_cast<EventDispatcher>(this->shared_from_this());
	}

	// Implementation of removing an event listener
	void EventDispatcher::removeListener(Ref<EventListenerBase> listener)
	{
		std::unique_lock<std::mutex> lock(mapMutex);
		auto range = listenerMap.equal_range(listener->getType());
		auto it = std::find_if(range.first, range.second, [&listener](const auto& pair) {
			return pair.second == listener;
			});

		assert(it != range.second);

		listenerMap.erase(it);

		listener->dispatcher = nullptr;
	}

	// Constructor for the event dispatcher
	EventDispatcher::EventDispatcher(uint32_t nThreads)
		: threadPool(nThreads), cToken(eventQueue)
	{
	}

	// Method to publish an event to the event queue
	void EventDispatcher::publishEvent(Ref<Event> event)
	{
		eventQueue.enqueue(event);
		std::unique_lock<std::mutex> lock(loopMutex);
		loopCv.notify_one();
	}

	// Method to publish events to the event queue
	void EventDispatcher::publishEvents(const std::vector<Ref<Event>>& events)
	{
		eventQueue.enqueue_bulk(events.data(), events.size());
		std::unique_lock<std::mutex> lock(loopMutex);
		loopCv.notify_one();
	}

	// Method to start the event loop
	void EventDispatcher::run()
	{
		running = true;

		// Buffer to hold events to process
		std::array<Ref<Event>, PATCHOULI_EVENT_BUFFER_SIZE> eventBuffer;

		while (running)
		{
			// Dequeue several events from the event queue
			std::size_t nEvents = eventQueue.wait_dequeue_bulk(cToken, eventBuffer.data(), eventBuffer.size());

			// Process each event
			for (std::size_t i = 0; i < nEvents; i++)
			{
				Ref<Event>& event = eventBuffer[i];
				EventType type = event->getType();

				// Special Event for event loop control
				switch (type)
				{
				case TerminationEvent::getStaticType():
					onTerminationEvent();
					break;
				case FenceEvent::getStaticType():
					onFenceEvent();
					break;
				}

				// Process background thread event listeners
				std::unique_lock<std::mutex> lock(mapMutex);
				auto range = listenerMap.equal_range(type);

				for (auto it = range.first; it != range.second; it++)
				{
					Ref<EventListenerBase>& listener = it->second;

					switch (event->getExecutionThread())
					{
					case Event::ExecutionPolicy::Inline:
						(*listener)(event);
						break;

					case Event::ExecutionPolicy::Background:
						beginEvent();
						threadPool.enqueue([=] { (*listener)(event); endEvent(); });
						break;
					}
				}
			}
		}
	}

	inline void EventDispatcher::beginEvent()
	{
		nTasks.fetch_add(1, std::memory_order_relaxed);
	}

	inline void EventDispatcher::endEvent()
	{
		if (nTasks.fetch_sub(1, std::memory_order_acq_rel) == 1)
		{
			std::unique_lock<std::mutex> lock(loopMutex);
			loopCv.notify_one();
		}
	}

	inline void EventDispatcher::onTerminationEvent()
	{
		running = false;
	}

	inline void EventDispatcher::onFenceEvent()
	{
		std::unique_lock<std::mutex> lock(loopMutex);
		loopCv.wait(lock, [this] { return nTasks.load(std::memory_order_acquire) <= 0; });
	}
}