#include "PObject/UUID.h"

namespace Patchouli
{
	static std::random_device rdLow;
	static std::random_device rdHigh;
	static std::mt19937_64 engLow(rdLow());
	static std::mt19937_64 engHigh(rdHigh());
	static std::uniform_int_distribution<uint64_t> uniformDistributionLow;
	static std::uniform_int_distribution<uint64_t> uniformDistributionHigh;


	UUID::UUID()
		: uuid({ uniformDistributionLow(engLow), uniformDistributionLow(engHigh) })
	{
	}

	UUID::UUID(const UUID&& other) noexcept
		: uuid(other.uuid)
	{
	}

	UUID& UUID::operator=(const UUID&& other) noexcept
	{
		this->uuid = other.uuid;
		return *this;
	}
}