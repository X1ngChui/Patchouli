#include "Core/PObject.h"
#include "Log/Console.h"
#include <mimalloc.h>

namespace Patchouli
{
	void* PObject::operator new(std::size_t size)
	{
		return mi_new(size);
	}

	void* PObject::operator new[](std::size_t size)
	{
		return mi_new(size);
	}

	void* PObject::operator new(std::size_t size, std::align_val_t al)
	{
		return mi_new_aligned(size, static_cast<std::size_t>(al));
	}

	void* PObject::operator new[](std::size_t size, std::align_val_t al)
	{
		return mi_new_aligned(size, static_cast<std::size_t>(al));
	}

	void* PObject::operator new(std::size_t size, const std::nothrow_t&) noexcept
	{
		return mi_new_nothrow(size);
	}

	void* PObject::operator new[](std::size_t size, const std::nothrow_t&) noexcept
	{
		return mi_new_nothrow(size);
	}

	void* PObject::operator new(std::size_t size, std::align_val_t al, const std::nothrow_t&) noexcept
	{
		return mi_new_aligned_nothrow(size, static_cast<std::size_t>(al));
	}

	void* PObject::operator new[](std::size_t size, std::align_val_t al, const std::nothrow_t&) noexcept
	{
		return mi_new_aligned_nothrow(size, static_cast<std::size_t>(al));
	}

	void PObject::operator delete(void* ptr)
	{
		mi_free(ptr);
	}

	void PObject::operator delete[](void* ptr)
	{
		mi_free(ptr);
	}

	void PObject::operator delete(void* ptr, std::align_val_t al)
	{
		mi_free_aligned(ptr, static_cast<std::size_t>(al));
	}

	void PObject::operator delete[](void* ptr, std::align_val_t al)
	{
		mi_free_aligned(ptr, static_cast<std::size_t>(al));
	}

	void PObject::operator delete(void* ptr, std::size_t size)
	{
		mi_free_size(ptr, size);
	}

	void PObject::operator delete[](void* ptr, std::size_t size)
	{
		mi_free_size(ptr, size);
	}

	void PObject::operator delete(void* ptr, std::size_t size, std::align_val_t al)
	{
		mi_free_size_aligned(ptr, size, static_cast<std::size_t>(al));
	}

	void PObject::operator delete[](void* ptr, std::size_t size, std::align_val_t al)
	{
		mi_free_size_aligned(ptr, size, static_cast<std::size_t>(al));
	}
}