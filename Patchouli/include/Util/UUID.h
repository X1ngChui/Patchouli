#pragma once

#include "Core/Base.h"

#ifdef _WIN64

#include <Windows.h>
#include <rpcdce.h>
#pragma comment(lib, "Rpcrt4.lib")

namespace Patchouli
{
	class PATCHOULI_API UUID
	{
	public:
		UUID()
		{
			(void)UuidCreate(&uuid);
		}

		~UUID() = default;

		UUID(const UUID& other)
			: uuid(other.uuid)
		{
		}

		UUID& operator=(const UUID& other)
		{
			uuid = other.uuid;
			return *this;
		}

		std::string toString() const
		{
			unsigned char* raw = nullptr;
			(void)UuidToStringA(&uuid, &raw);

			constexpr auto deleter = [](unsigned char* ptr) { RpcStringFreeA(&ptr);};
			std::unique_ptr<unsigned char, decltype(deleter)> rawPtr(raw, deleter);

			return std::string(reinterpret_cast<char*>(rawPtr.get()));
		}

		auto operator<=>(const UUID& other) const
		{
			RPC_STATUS status;
			int result = UuidCompare(const_cast<::UUID*>(&uuid), const_cast<::UUID*>(&other.uuid), &status);
			if (result < 0)
				return std::strong_ordering::less;
			else if (result > 0)
				return std::strong_ordering::greater;

			return std::strong_ordering::equal;
		}

		bool operator==(const UUID& other) const
		{
			RPC_STATUS status;
			int result = UuidCompare(const_cast<::UUID*>(&uuid), const_cast<::UUID*>(&other.uuid), &status);
			return result == 0;
		}

		bool operator!=(const UUID& other) const
		{
			RPC_STATUS status;
			int result = UuidCompare(const_cast<::UUID*>(&uuid), const_cast<::UUID*>(&other.uuid), &status);
			return result != 0;
		}

		bool operator<(const UUID& other) const
		{
			RPC_STATUS status;
			int result = UuidCompare(const_cast<::UUID*>(&uuid), const_cast<::UUID*>(&other.uuid), &status);
			return result < 0;
		}

		bool operator<=(const UUID& other) const
		{
			RPC_STATUS status;
			int result = UuidCompare(const_cast<::UUID*>(&uuid), const_cast<::UUID*>(&other.uuid), &status);
			return result <= 0;
		}

		bool operator>(const UUID& other) const
		{
			RPC_STATUS status;
			int result = UuidCompare(const_cast<::UUID*>(&uuid), const_cast<::UUID*>(&other.uuid), &status);
			return result > 0;
		}

		bool operator>=(const UUID & other) const
		{
			RPC_STATUS status;
			int result = UuidCompare(const_cast<::UUID*>(&uuid), const_cast<::UUID*>(&other.uuid), &status);
			return result >= 0;
		}
	private:
		::UUID uuid;
	};
}

#endif