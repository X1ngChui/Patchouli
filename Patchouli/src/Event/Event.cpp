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
		std::array<Ref<Event>, PATCHOULI_EVENT_BUFFER_SIZE> eventBuffer;

		while (running)
		{
			// Attempt to dequeue events from the event queue
			std::size_t nEvents = eventQueue.wait_dequeue_bulk(getConsumerToken(), eventBuffer.data(),  eventBuffer.size());
	
			// Process each event
			for (std::size_t i = 0; i < nEvents; i++)
			{
				Ref<Event> event = eventBuffer[i];
				EventTypeID type = event->getType();

				// Special Event for event loop control
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
				auto range = listenerMap.equal_range(type);

				for (auto it = range.first; it != range.second; it++)
				{
					Ref<EventListenerBase> listener = it->second;

					switch (event->getExecutionThread())
					{
					case Event::ExecutionThread::Main:
						(*listener)(event);
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
