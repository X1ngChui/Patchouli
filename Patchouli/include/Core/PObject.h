#pragma once

#include "Core/Base.h"
#include "Util/Reference.h"
#include <concurrentqueue.h>

namespace Patchouli
{
    class PATCHOULI_API PObject : public RefBase<PObject>
    {
    public:
        PObject() noexcept = default;
        virtual ~PObject() noexcept = default;

        // Overloaded new operators for fast memory management
        void* operator new(std::size_t size);
        void* operator new[](std::size_t size);
        void* operator new(std::size_t size, std::align_val_t al);
        void* operator new[](std::size_t size, std::align_val_t a);
        void* operator new(std::size_t size, const std::nothrow_t&) noexcept;
        void* operator new[](std::size_t size, const std::nothrow_t&) noexcept;
        void* operator new(std::size_t size, std::align_val_t al, const std::nothrow_t&) noexcept;
        void* operator new[](std::size_t size, std::align_val_t al, const std::nothrow_t&) noexcept;

        // Overloaded delete operators for fast memory management
        void operator delete(void* ptr) noexcept;
        void operator delete[](void* ptr) noexcept;
        void operator delete(void* ptr, std::align_val_t al) noexcept;
        void operator delete[](void* ptr, std::align_val_t al) noexcept;
        void operator delete(void* ptr, std::size_t size) noexcept;
        void operator delete[](void* ptr, std::size_t size) noexcept;
        void operator delete(void* ptr, std::size_t size, std::align_val_t al) noexcept;
        void operator delete[](void* ptr, std::size_t size, std::align_val_t al) noexcept;
    };
}