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
		ListenerMap::accessor accessor;
		if (!listeners.find(accessor, eventTypeID))
			listeners.insert(accessor, eventTypeID);
		accessor->second.push_back(listener);

		listener->dispatcher = weak_from_this();
	}

	void EventDispatcher::removeListenerImpl(EventTypeID eventTypeID, EventListenerBase* listener)
	{
		ListenerMap::accessor accessor;
		bool found = listeners.find(accessor, eventTypeID);
		assert(found);

		auto& listenerList = accessor->second;

		auto it = std::ranges::find_if(listenerList, [listener](Ref<EventListenerBase> l)
			{ return l.get() == listener; }
		);
		if (it != listenerList.end())
			listenerList.erase(it);

		listener->dispatcher = nullptr;
	}

	void EventDispatcher::dispatch(Event& event)
	{
		auto eventTypeID = event.getType();

		ListenerMap::accessor accessor;
		if (listeners.find(accessor, eventTypeID))
		{
			const auto& listenerList = accessor->second;

			for (auto it = listenerList.cbegin(); it != listenerList.cend(); it++)
				(**it)(event);
		}
	}
}