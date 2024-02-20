#pragma once

#include "PatchouliPch.h"
#include "PObject/UUID.h"
#include "PObject/IntrusivePtr.h"

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
	 * This class serves as the base class for all Patchouli objects and provides
	 * functionality for uniquely identifying objects using UUIDs (Universally Unique Identifiers).
	 */
	class PObject : public std::intrusive_base<PObject>
	{
	public:
		virtual ~PObject() = default;

		bool operator==(const PObject& other) const { return this->uuid == other.uuid; }
		bool operator!=(const PObject& other) const { return this->uuid != other.uuid; }

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

		PObject(const PObject& other) = delete;
		PObject& operator=(const PObject& other) = delete;
	private:
		friend struct std::hash<PObject>;
		UUID uuid;	/**< The UUID (Universally Unique Identifier) of the Patchouli object. */
	};
}

namespace std {

	template <>
	struct hash<Patchouli::PObject>
	{
		std::size_t operator()(const Patchouli::PObject& obj) const
		{
			return std::hash<Patchouli::UUID>()(obj.uuid);
		}
	};
}