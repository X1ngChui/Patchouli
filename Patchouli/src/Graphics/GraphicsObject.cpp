#include "Graphics/GraphicsObject.h"
#include <mimalloc.h>

namespace Patchouli
{
	void* GraphicsObject::operator new(std::size_t size)
	{
		return mi_new(size);
	}

	void* GraphicsObject::operator new[](std::size_t size)
	{
		return mi_new(size);
	}

	void* GraphicsObject::operator new(std::size_t size, const std::nothrow_t& tag) noexcept
	{
		(void)(tag);
		return mi_new_nothrow(size);
	}

	void* GraphicsObject::operator new[](std::size_t size, const std::nothrow_t& tag) noexcept
	{
		(void)(tag);
		return mi_new_nothrow(size);
	}

	void GraphicsObject::operator delete(void* ptr)
	{
		mi_free(ptr);
	}

	void GraphicsObject::operator delete[](void* ptr)
	{
		mi_free(ptr);
	}

	void GraphicsObject::operator delete(void* ptr, const std::nothrow_t& tag) noexcept
	{
		(void)(tag);
		mi_free(ptr);
	}

	void GraphicsObject::operator delete[](void* ptr, const std::nothrow_t& tag) noexcept
	{
		(void)(tag);
		mi_free(ptr);
	}
}
