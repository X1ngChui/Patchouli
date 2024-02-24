#include "Util/UUID.h"

#ifdef _WIN64

namespace Patchouli
{
	UUID::UUID()
	{
		UuidCreate(&uuid);
	}

	UUID::UUID(const UUID& other)
		: uuid(other.uuid)
	{
	}

	UUID& UUID::operator=(const UUID& other)
	{
		uuid = other.uuid;
		return *this;
	}

	std::string UUID::toString() const
	{
		unsigned char* raw = nullptr;
		(void)UuidToStringA(&uuid, &raw);

		constexpr auto deleter = [](unsigned char* ptr) { RpcStringFreeA(&ptr); };
		std::unique_ptr<unsigned char, decltype(deleter)> rawPtr(raw, deleter);

		return std::string(reinterpret_cast<char*>(rawPtr.get()));
	}

	auto UUID::operator<=>(const UUID& other) const
	{
		RPC_STATUS status;
		int result = UuidCompare(const_cast<::UUID*>(&uuid), const_cast<::UUID*>(&other.uuid), &status);
		if (result < 0)
			return std::strong_ordering::less;
		else if (result > 0)
			return std::strong_ordering::greater;
		return std::strong_ordering::equal;
	}

	bool UUID::operator==(const UUID& other) const
	{
		RPC_STATUS status;
		int result = UuidCompare(const_cast<::UUID*>(&uuid), const_cast<::UUID*>(&other.uuid), &status);
		return result == 0;
	}

	bool UUID::operator!=(const UUID& other) const
	{
		RPC_STATUS status;
		int result = UuidCompare(const_cast<::UUID*>(&uuid), const_cast<::UUID*>(&other.uuid), &status);
		return result != 0;
	}

	bool UUID::operator<(const UUID& other) const
	{
		RPC_STATUS status;
		int result = UuidCompare(const_cast<::UUID*>(&uuid), const_cast<::UUID*>(&other.uuid), &status);
		return result < 0;
	}

	bool UUID::operator<=(const UUID& other) const
	{
		RPC_STATUS status;
		int result = UuidCompare(const_cast<::UUID*>(&uuid), const_cast<::UUID*>(&other.uuid), &status);
		return result <= 0;
	}

	bool UUID::operator>(const UUID& other) const
	{
		RPC_STATUS status;
		int result = UuidCompare(const_cast<::UUID*>(&uuid), const_cast<::UUID*>(&other.uuid), &status);
		return result > 0;
	}

	bool UUID::operator>=(const UUID& other) const
	{
		RPC_STATUS status;
		int result = UuidCompare(const_cast<::UUID*>(&uuid), const_cast<::UUID*>(&other.uuid), &status);
		return result >= 0;
	}
}

#elif defined(__linux)

namespace Patchouli
{
	UUID::UUID()
	{
		uuid_generate(uuid);
	}

	UUID::UUID(const UUID& other)
	{
		uuid_copy(uuid, other.uuid);
	}

	UUID& UUID::operator=(const UUID& other)
	{
		uuid_copy(uuid, other.uuid);
		return *this;
	}

	std::string UUID::toString() const
	{
		char str[37];
		uuid_unparse(uuid, str);
		return std::string(str);
	}

	auto UUID::operator<=>(const UUID& other) const
	{
		int result = uuid_compare(uuid, other.uuid);
		if (result < 0)
			return std::strong_ordering::less;
		else if (result > 0)
			return std::strong_ordering::greater;
		return std::strong_ordering::equal;
	}

	bool UUID::operator==(const UUID& other) const
	{
		return uuid_compare(uuid, other.uuid) == 0;
	}

	bool UUID::operator!=(const UUID& other) const
	{
		return uuid_compare(uuid, other.uuid) != 0;
	}

	bool UUID::operator<(const UUID& other) const
	{
		return uuid_compare(uuid, other.uuid) < 0;
	}

	bool UUID::operator<=(const UUID& other) const
	{
		return uuid_compare(uuid, other.uuid) <= 0;
	}

	bool UUID::operator>(const UUID& other) const
	{
		return uuid_compare(uuid, other.uuid) > 0;
	}

	bool UUID::operator>=(const UUID& other) const
	{
		return uuid_compare(uuid, other.uuid) >= 0;
	}
}

#endif