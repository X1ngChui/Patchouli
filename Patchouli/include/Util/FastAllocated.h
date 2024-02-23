#pragma once

#include "Core/Base.h"

namespace Patchouli
{
	struct PATCHOULI_API FastAllocated
	{
		/* Memory pooling */
		void* operator new(std::size_t size);
		void* operator new[](std::size_t size);
		void* operator new(std::size_t size, const std::nothrow_t& tag) noexcept;
		void* operator new[](std::size_t size, const std::nothrow_t& tag) noexcept;

		void operator delete(void* ptr);
		void operator delete[](void* ptr);
		void operator delete(void* ptr, const std::nothrow_t&) noexcept;
		void operator delete[](void* ptr, const std::nothrow_t&) noexcept;
	};
}