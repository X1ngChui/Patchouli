#pragma once

#include <cstdint>
#include <string_view>

#if defined __clang__ || defined __GNUC__
#	define PATCHOULI_PRETTY_FUNCTION __PRETTY_FUNCTION__
#	define PATCHOULI_PRETTY_FUNCTION_PREFIX '='
#	define PATCHOULI_PRETTY_FUNCTION_SUFFIX ']'
#elif defined _MSC_VER
#	define PATCHOULI_PRETTY_FUNCTION __FUNCSIG__
#	define PATCHOULI_PRETTY_FUNCTION_PREFIX '<'
#	define PATCHOULI_PRETTY_FUNCTION_SUFFIX '>'
#endif

#define PATCHOULI_TYPE_HASH_PRIME 1099511628211ull
#define PATCHOULI_TYPE_HASH_OFFSET 14695981039346656037ull

namespace Patchouli
{
	template <typename T>
	constexpr auto typeName() noexcept
	{
		std::string_view prettyFunction{ PATCHOULI_PRETTY_FUNCTION };
		auto first = prettyFunction.find_first_not_of(' ', prettyFunction.find_first_of(PATCHOULI_PRETTY_FUNCTION_PREFIX) + 1);
		auto value = prettyFunction.substr(first, prettyFunction.find_last_of(PATCHOULI_PRETTY_FUNCTION_SUFFIX) - first);
		return value;
	}

	template <typename T>
	constexpr auto typeHash()
	{
		uint64_t hash = PATCHOULI_TYPE_HASH_OFFSET;
		auto name = typeName<T>();
		for (char c : name)
			hash = (hash ^ c) * PATCHOULI_TYPE_HASH_PRIME;
		return hash;
	}

	template <typename T>
	struct TypeTraits
	{
		static constexpr auto name = typeName<T>();
		static constexpr auto hash = typeHash<T>();
	};

}