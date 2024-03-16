#include "Event/Event.h"

#define PATCHOULI_EVENT_BUFFER_SIZE 16

namespace Patchouli
{
	void EventListenerBase::operator()(Ref<Event> event) const
	{
		callback(event);
	}

	void EventDispatcher::addListenerImpl(EventTypeID eventTypeID, Ref<EventListenerBase> listener)
	{
		std::unique_lock<std::mutex> lock(mapMutex);
		listenerMap[eventTypeID].push_back(listener);

		listener->dispatcher = std::static_pointer_cast<EventDispatcher>(this->shared_from_this());
	}

	void EventDispatcher::removeListenerImpl(EventTypeID eventTypeID, EventListenerBase* listener)
	{
		std::unique_lock<std::mutex> lock(mapMutex);

		auto& listenerList = listenerMap[eventTypeID];

		auto it = std::ranges::find_if(listenerList, [listener](Ref<EventListenerBase> l)
			{ return l.get() == listener; }
		);
		if (it != listenerList.end())
			listenerList.erase(it);

		listener->dispatcher = nullptr;
	}

	EventDispatcher::EventDispatcher(uint32_t nThreads)
		: token(eventQueue), nThreads(nThreads), threadPool(nThreads)
	{
	}

	void EventDispatcher::publishEvent(Ref<Event> event)
	{
		eventQueue.enqueue(event);
		cv.notify_one();
	}

	void EventDispatcher::run()
	{
		running = true;

		Ref<Event> eventBuffer[PATCHOULI_EVENT_BUFFER_SIZE];

		while (running)
		{
			std::size_t nEvents = eventQueue.try_dequeue_bulk(token, eventBuffer, PATCHOULI_EVENT_BUFFER_SIZE);

			if (nEvents <= 0)
			{
				std::unique_lock<std::mutex> lock(threadMutex);
				cv.wait(lock);
				continue;
			}

			std::unique_lock<std::mutex> mapLock(mapMutex);

			for (uint32_t i = 0; i < nEvents; i++)
			{
				Ref<Event> event = eventBuffer[i];
				const auto& listenerList = listenerMap[event->getType()];

				for (auto it = listenerList.cbegin(); it != listenerList.cend(); it++)
				{
					switch ((*it)->getExecutionThread())
					{
					case EventListenerBase::ExecutionThread::Main:
						(**it)(event);
						break;
					case EventListenerBase::ExecutionThread::Background:
						threadPool.addWorkFunc([=] { (**it)(event); });
						break;
					}
				}
			}
		}
	}

	void EventDispatcher::stop()
	{
		running = false;
		cv.notify_one();
	}
}