#pragma once

#include "PatchouliPch.h"
#include "Core/Base.h"
#include "Util/intrusive_ptr.h"

namespace Patchouli
{
	/* Pointer wrappers */

	template <typename T>
	using Scope = std::unique_ptr<T>;

	template <typename T, typename... Args>
	constexpr Scope<T> makeScope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using Ref = std::intrusive_ptr<T>;

	template <typename T, typename... Args>
	constexpr Ref<T> makeRef(Args&&... args)
	{
		return std::make_intrusive<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using WeakRef = std::intrusive_weak_ptr<T>;

	/**
	 * @brief Base class representing a Patchouli object.
     *
	 * This class serves as the foundation for all Patchouli objects,
	 * offering integrated memory pooling and reference counting features.
	 */
	class PATCHOULI_API PObject : public std::intrusive_base<PObject>
	{
	public:
		virtual ~PObject() = default;

		/* Memory pooling */
		void* operator new(std::size_t size);
		void* operator new[](std::size_t size);
		void* operator new(std::size_t size, const std::nothrow_t& tag) noexcept;
		void* operator new[](std::size_t size, const std::nothrow_t& tag) noexcept;

		void operator delete(void* ptr);
		void operator delete[](void* ptr);
		void operator delete(void* ptr, const std::nothrow_t&) noexcept;
		void operator delete[](void* ptr, const std::nothrow_t&) noexcept;
	protected:
		PObject() = default;
	};
}