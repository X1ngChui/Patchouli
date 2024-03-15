#pragma once

#include "Core/Base.h"
#include "Util/Reference.h"

namespace Patchouli
{
	class PATCHOULI_API GraphicsObject : public RefBase<GraphicsObject>
	{
	public:
		GraphicsObject() = default;
		virtual ~GraphicsObject() = default;

		void* operator new(std::size_t size);
		void* operator new[](std::size_t size);
		void* operator new(std::size_t size, const std::nothrow_t& tag) noexcept;
		void* operator new[](std::size_t size, const std::nothrow_t& tag) noexcept;

		void operator delete(void* ptr);
		void operator delete[](void* ptr);
		void operator delete(void* ptr, const std::nothrow_t& tag) noexcept;
		void operator delete[](void* ptr, const std::nothrow_t& tag) noexcept;
	};
}