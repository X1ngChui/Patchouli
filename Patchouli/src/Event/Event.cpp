#include "Event/Event.h"

// Maximum number of events to process in a single iteration
#define PATCHOULI_EVENT_BUFFER_SIZE 16

namespace Patchouli
{
	// Operator overload to invoke the event callback
	void EventListenerBase::operator()(Ref<Event> event) const
	{
		callback(event);
	}

	// Implementation of adding an event listener
	void EventDispatcher::addListenerImpl(EventTypeID eventTypeID, Ref<EventListenerBase> listener)
	{
		switch (listener->getExecutionThread())
		{
		case EventListenerBase::ExecutionThread::Main:
		{
			std::unique_lock<std::mutex> mainLock(mainMapMutex);
			mainListenerMap[eventTypeID].push_back(listener);
			break;
		}
		case EventListenerBase::ExecutionThread::Background:
		{
			std::unique_lock<std::mutex> backgroundLock(backgroundMapMutex);
			backgroundListenerMap[eventTypeID].push_back(listener);
			break;
		}
		}

		listener->dispatcher = std::static_pointer_cast<EventDispatcher>(this->shared_from_this());
	}

	// Implementation of removing an event listener
	void EventDispatcher::removeListenerImpl(EventTypeID eventTypeID, EventListenerBase* listener)
	{
		switch (listener->getExecutionThread())
		{
		case EventListenerBase::ExecutionThread::Main:
		{
			std::unique_lock<std::mutex> mainLock(mainMapMutex);

			auto& listenerList = mainListenerMap[eventTypeID];

			auto it = std::ranges::find_if(listenerList, [listener](Ref<EventListenerBase> l)
				{ return l.get() == listener; }
			);
			if (it != listenerList.end())
				listenerList.erase(it);
			break;
		}
		case EventListenerBase::ExecutionThread::Background:
		{
			std::unique_lock<std::mutex> backgroundLock(backgroundMapMutex);

			auto& listenerList = backgroundListenerMap[eventTypeID];

			auto it = std::ranges::find_if(listenerList, [listener](Ref<EventListenerBase> l)
				{ return l.get() == listener; }
			);
			if (it != listenerList.end())
				listenerList.erase(it);
			break;
		}
		}

		listener->dispatcher = nullptr;
	}

	// Constructor for the event dispatcher
	EventDispatcher::EventDispatcher(uint32_t nThreads)
		: token(eventQueue), nThreads(nThreads), threadPool(nThreads)
	{
	}

	// Method to publish an event to the event queue
	void EventDispatcher::publishEvent(Ref<Event> event)
	{
		eventQueue.enqueue(event);
		cv.notify_one();
	}

	// Method to start the event loop
	void EventDispatcher::run()
	{
		running = true;

		// Buffer to hold events to process
		Ref<Event> eventBuffer[PATCHOULI_EVENT_BUFFER_SIZE];

		while (running)
		{
			// Attempt to dequeue events from the event queue
			std::size_t nEvents = eventQueue.try_dequeue_bulk(token, eventBuffer, PATCHOULI_EVENT_BUFFER_SIZE);

			if (nEvents <= 0)
			{
				// Wait if no events are available
				std::unique_lock<std::mutex> lock(threadMutex);
				cv.wait(lock);
				continue;
			}

			// Process each event
			for (uint32_t i = 0; i < nEvents; i++)
			{
				Ref<Event> event = eventBuffer[i];

				{
					// Process main thread event listeners
					std::unique_lock<std::mutex> mainLock(mainMapMutex);
					const auto& listenerList = mainListenerMap[event->getType()];

					for (auto it = listenerList.cbegin(); it != listenerList.cend(); it++)
						(**it)(event);
				}

				{
					// Process background thread event listeners
					std::unique_lock<std::mutex> backgroundLock(backgroundMapMutex);
					const auto& listenerList = backgroundListenerMap[event->getType()];
					for (auto it = listenerList.cbegin(); it != listenerList.cend(); it++)
						threadPool.addWorkFunc([=] { (**it)(event); });
				}
			}
		}
	}

	// Method to stop the event loop
	void EventDispatcher::stop()
	{
		running = false;
		cv.notify_one();
	}
}
