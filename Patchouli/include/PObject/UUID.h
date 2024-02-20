#pragma once
#include "PatchouliPch.h"

namespace Patchouli
{
	// UUID (universally unique identifier) is a 128-bit integer to "uniquely" identify an object.
	// At the moment we're simply using a randomly generated 128-bit integer, as the possibility of
	// a clash is low enough for now. This may change in the future.
	class UUID
	{
	public:
		UUID();
		UUID(const UUID&& other) noexcept;
		UUID& operator=(const UUID&& other) noexcept;

		bool operator==(const UUID& other) const { return this->uuid == other.uuid; }
		bool operator!=(const UUID& other) const { return this->uuid != other.uuid; }
	private:
		friend struct std::hash<Patchouli::UUID>;

		struct alignas(16) uint128_t
		{
			uint64_t low;
			uint64_t high;

			bool operator==(const uint128_t other) const { return this->low == other.low && this->high == other.high; }
			bool operator!=(const uint128_t other) const { return this->low != other.low || this->high != other.high; }
		};

		uint128_t uuid;
	};
}

namespace std {

	template <>
	struct hash<Patchouli::UUID>
	{
		std::size_t operator()(const Patchouli::UUID& uuid) const
		{
			// UUID is already a random number and suitable as a hash key as-is.
			// This may change in the future.
			return uuid.uuid.low ^ uuid.uuid.high;
		}
	};
}