#include "Core/PObject.h"
#include "mimalloc.h"

namespace Patchouli
{
	void* FastMemory::operator new(std::size_t size)
	{
		return mi_new(size);
	}

	void* FastMemory::operator new[](std::size_t size)
	{
		return mi_new(size);
	}

	void* FastMemory::operator new(std::size_t size, const std::nothrow_t& tag) noexcept
	{
		(void)(tag);
		return mi_new_nothrow(size);
	}

	void* FastMemory::operator new[](std::size_t size, const std::nothrow_t& tag) noexcept
	{
		(void)(tag);
		return mi_new_nothrow(size);
	}

	void FastMemory::operator delete(void* ptr)
	{
		mi_free(ptr);
	}

	void FastMemory::operator delete[](void* ptr)
	{
		mi_free(ptr);
	}

	void FastMemory::operator delete(void* ptr, const std::nothrow_t&) noexcept
	{
		mi_free(ptr);
	}

	void FastMemory::operator delete[](void* ptr, const std::nothrow_t&) noexcept
	{
		mi_free(ptr);
	}
}