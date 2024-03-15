#include "Event/Event.h"
#include <mimalloc.h>

namespace Patchouli
{
	void* Event::operator new(std::size_t size)
	{
		return mi_new(size);
	}

	void* Event::operator new[](std::size_t size)
	{
		return mi_new(size);
	}

	void* Event::operator new(std::size_t size, const std::nothrow_t& tag) noexcept
	{
		(void)(tag);
		return mi_new_nothrow(size);
	}

	void* Event::operator new[](std::size_t size, const std::nothrow_t& tag) noexcept
	{
		(void)(tag);
		return mi_new_nothrow(size);
	}

	void Event::operator delete(void* ptr)
	{
		mi_free(ptr);
	}

	void Event::operator delete[](void* ptr)
	{
		mi_free(ptr);
	}

	void Event::operator delete(void* ptr, const std::nothrow_t& tag) noexcept
	{
		(void)(tag);
		mi_free(ptr);
	}

	void Event::operator delete[](void* ptr, const std::nothrow_t& tag) noexcept
	{
		(void)(tag);
		mi_free(ptr);
	}

	void EventListenerBase::operator()(Ref<Event> event) const
	{
		callback(event);
	}

	void EventDispatcher::addListenerImpl(EventTypeID eventTypeID, Ref<EventListenerBase> listener)
	{
		std::unique_lock<std::mutex> lock(mapMutex);
		listenerMap[eventTypeID].push_back(listener);

		listener->dispatcher = weak_from_this();
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

	EventDispatcher::EventDispatcher()
		: running(true), dispatcherThread([this] { dispatchLoop(); })
	{
	}

	EventDispatcher::~EventDispatcher()
	{
		running = false;
		dispatcherThread.join();
	}

	void EventDispatcher::publishEvent(Ref<Event> event)
	{
		eventQueue.enqueue(event);
		cv.notify_one();
	}

	void EventDispatcher::dispatch()
	{
		Ref<Event> event = nullptr;

		std::unique_lock<std::mutex> lock(mapMutex);
		while (eventQueue.try_dequeue(event))
		{ 
			const auto& listenerList = listenerMap[event->getType()];

			for (auto it = listenerList.cbegin(); it != listenerList.cend(); it++)
				(**it)(event);
		}
	}

	void EventDispatcher::dispatchLoop()
	{
		while (running)
		{
			std::unique_lock<std::mutex> lock(threadMutex);
			cv.wait(lock, [] { return true; });

			dispatch();
		}
	}
}