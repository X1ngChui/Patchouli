#pragma once

#include "Util/intrusive_ptr.h"
#include "Util/UUID.h"

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

	template <typename T, typename D = std::default_delete<T>>
	using RefBase = std::intrusive_base<T, D>;

	template <typename T>
	using Ref = std::intrusive_ptr<T>;

	template <typename T, typename... Args>
	constexpr Ref<T> makeRef(Args&&... args)
	{
		return std::make_intrusive<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using WeakRef = std::intrusive_weak_ptr<T>;
}