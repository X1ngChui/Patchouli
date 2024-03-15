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

	void EventListenerBase::operator()(Event& event) const
	{
		callback(event);
	}

	void EventDispatcher::addListenerImpl(EventTypeID eventTypeID, Ref<EventListenerBase> listener)
	{
		listeners[eventTypeID].push_back(listener);
		listener->dispatcher = this->weak_from_this();
	}

	void EventDispatcher::removeListenerImpl(EventTypeID eventTypeID, EventListenerBase* listener)
	{
		auto& listenerList = listeners[eventTypeID];

		auto it = std::ranges::find_if(listenerList, [listener](Ref<EventListenerBase> l)
			{ return l.get() == listener; }
		);
		if (it != listenerList.end())
			listenerList.erase(it);

		listener->dispatcher = nullptr;
	}

	void EventDispatcher::dispatch(Event& event)
	{
		auto type = event.getType();
		auto& listenerList = listeners[event.getType()];

		for (auto it = listenerList.begin(); it != listenerList.end(); it++)
			(**it)(event);
	}
}