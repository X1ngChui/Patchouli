#include "Event/Event.h"
#include "Event/FenceEvent.h"
#include "Event/TerminationEvent.h"

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
	void EventDispatcher::addListener(Ref<EventListenerBase> listener)
	{
		std::unique_lock<std::mutex> lock(mapMutex);
		listenerMap[listener->getType()].push_back(listener);

		listener->dispatcher = std::static_pointer_cast<EventDispatcher>(this->shared_from_this());
	}

	// Implementation of removing an event listener
	void EventDispatcher::removeListener(Ref<EventListenerBase> listener)
	{
		
		std::unique_lock<std::mutex> lock(mapMutex);

		auto& listenerList = listenerMap[listener->getType()];

		auto it = std::ranges::find(listenerList, listener);
		if (it != listenerList.end())
			listenerList.erase(it);

		listener->dispatcher = nullptr;
	}

	// Constructor for the event dispatcher
	EventDispatcher::EventDispatcher(uint32_t nThreads)
		: threadPool(nThreads), nTasks(0)
	{
	}

	// Method to publish an event to the event queue
	void EventDispatcher::publishEvent(Ref<Event> event)
	{
		eventQueue.enqueue(getProducerToken(), event);
		std::unique_lock<std::mutex> lock(loopMutex);
		loopCv.notify_one();
	}

	void EventDispatcher::publishEvents(const std::vector<Ref<Event>>& events)
	{
		eventQueue.enqueue_bulk(getProducerToken(), events.data(), events.size());
		std::unique_lock<std::mutex> lock(loopMutex);
		loopCv.notify_one();
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
			std::size_t nEvents = eventQueue.wait_dequeue_bulk(getConsumerToken(), eventBuffer, PATCHOULI_EVENT_BUFFER_SIZE);

			assert(nEvents > 0);
	
			// Process each event
			for (std::size_t i = 0; i < nEvents; i++)
			{
				Ref<Event> event = eventBuffer[i];
				EventTypeID type = event->getType();

				switch (type) {
					case TerminationEvent::EventType:
						running = false;
						break;

					case FenceEvent::EventType:
					{
						std::unique_lock<std::mutex> lock(loopMutex);
						loopCv.wait(lock, [this] { return nTasks.load(std::memory_order_acquire) <= 0; });
					}
						break;
				}

				// Process background thread event listeners
				std::unique_lock<std::mutex> lock(mapMutex);
				const auto& listenerList = listenerMap[type];

				for (auto it = listenerList.cbegin(); it != listenerList.cend(); it++)
				{
					Ref<EventListenerBase> listener = *it;

					switch (event->getExecutionThread())
					{
					case Event::ExecutionThread::Main:
						beginEvent();
						(*listener)(event);
						endEvent();
						break;
					case Event::ExecutionThread::Background:
						beginEvent();
						threadPool.enqueue([=] {
								(*listener)(event);
								endEvent();
							});
						break;
					}
				}
			}
		}
	}

	void EventDispatcher::beginEvent()
	{
		nTasks.fetch_add(1, std::memory_order_relaxed);
	}

	void EventDispatcher::endEvent()
	{
		if (nTasks.fetch_sub(1, std::memory_order_acq_rel) == 1)
		{
			std::unique_lock<std::mutex> lock(loopMutex);
			loopCv.notify_one();
		}
	}

	moodycamel::ProducerToken& EventDispatcher::getProducerToken()
	{
		static thread_local moodycamel::ProducerToken pToken(eventQueue);
		return pToken;
	}

	moodycamel::ConsumerToken& EventDispatcher::getConsumerToken()
	{
		static thread_local moodycamel::ConsumerToken cToken(eventQueue);
		return cToken;
	}
}
