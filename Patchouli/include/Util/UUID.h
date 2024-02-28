#pragma once

#include "Core/Base.h"
#include <fmt/format.h>

#ifdef _WIN64
	#include <Windows.h>
	#include <rpcdce.h>
	#pragma comment(lib, "Rpcrt4.lib")
#elif defined(__linux)
	#include <uuid/uuid.h>
#endif

namespace Patchouli
{
	/* @brief This class represents Universally Unique Identifiers(UUIDs).
	 * It provides functionality to generate, compare, and convert UUIDs to string format.
	 */
	class PATCHOULI_API UUID
	{
	public:
		UUID();
		~UUID() = default;

		UUID(const UUID& other);
		UUID& operator=(const UUID& other);

		// Convert the UUID to a string representation.
		std::string toString() const;
		explicit operator std::string() const { return toString(); }

		// Comparison operators for UUIDs.
		auto operator<=>(const UUID& other) const;
		bool operator==(const UUID& other) const;
		bool operator!=(const UUID& other) const;
		bool operator<(const UUID& other) const;
		bool operator<=(const UUID& other) const;
		bool operator>(const UUID& other) const;
		bool operator>=(const UUID& other) const;
	private:
#ifdef _WIN64
		::UUID uuid;
#elif defined(__linux)
		uuid_t uuid;
#endif
	};
}

template<>
struct fmt::formatter<Patchouli::UUID> : fmt::formatter<std::string>
{
	template <typename FormatCtx>
	auto format(const Patchouli::UUID& uuid, FormatCtx& ctx) const
	{
		return fmt::formatter<std::string>::format(uuid.toString(), ctx);
	}
};
