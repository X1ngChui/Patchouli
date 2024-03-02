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
}