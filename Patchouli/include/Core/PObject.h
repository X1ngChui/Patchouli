#pragma once

#include "Core/Base.h"
#include "Util/Reference.h"

namespace Patchouli
{
	struct PATCHOULI_API PObject : RefBase<PObject>
	{
        virtual ~PObject() = default;

        // Overloaded new operators for memory management
        void* operator new(std::size_t size);
        void* operator new[](std::size_t size);
        void* operator new(std::size_t size, const std::nothrow_t& tag) noexcept;
        void* operator new[](std::size_t size, const std::nothrow_t& tag) noexcept;

        // Overloaded delete operators for memory management
        void operator delete(void* ptr);
        void operator delete[](void* ptr);
        void operator delete(void* ptr, const std::nothrow_t& tag) noexcept;
        void operator delete[](void* ptr, const std::nothrow_t& tag) noexcept;
	};
}