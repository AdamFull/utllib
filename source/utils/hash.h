#pragma once

#include "reflection.h"

namespace utl
{
	namespace utils
	{
        constexpr uint64_t fnv_prime = 1099511628211ull;
        constexpr uint64_t fnv_offset_basis = 14695981039346656037ull;

        constexpr inline uint64_t fnv1a_64_hash(const char* str, uint64_t hash = fnv_offset_basis) noexcept
        {
            while (*str != '\0')
            {
                hash = (hash ^ cast<uint64_t>(*str)) * fnv_prime;
                str++;
            }
            return hash;
        }

        constexpr inline uint32_t const_hash(char const* input) noexcept
        {
            return *input ? cast<uint32_t>(*input) + 33 * const_hash(input + 1) : 5381u;
        }

        constexpr inline uint32_t const_hash(char const* s, uint64_t count) noexcept
        {
            return ((count ? const_hash(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u; // NOLINT (hicpp-signed-bitwise)
        }

        constexpr inline uint32_t operator "" _utl_hash(char const* s, uint64_t count) noexcept
        {
            return const_hash(s, count);
        }

        template<class _Ty>
        constexpr inline uint32_t type_hash() noexcept
        {
            constexpr auto typestr = type_name<_Ty>();
            return const_hash(typestr.data());
        }
	}
}