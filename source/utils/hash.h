#pragma once

#include "reflection.h"

namespace utl
{
	namespace utils
	{
        constexpr u64 fnv_prime = 1099511628211ull;
        constexpr u64 fnv_offset_basis = 14695981039346656037ull;

        constexpr inline u64 fnv1a_64_hash(const char* str, u64 hash = fnv_offset_basis) noexcept
        {
            while (*str != '\0')
            {
                hash = (hash ^ cast<u64>(*str)) * fnv_prime;
                str++;
            }
            return hash;
        }

        constexpr inline u32 const_hash(char const* input) noexcept
        {
            return *input ? cast<u32>(*input) + 33 * const_hash(input + 1) : 5381u;
        }

        constexpr inline u32 const_hash(char const* s, u64 count) noexcept
        {
            return ((count ? const_hash(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u; // NOLINT (hicpp-signed-bitwise)
        }

        constexpr inline u32 operator "" _utl_hash(char const* s, u64 count) noexcept
        {
            return const_hash(s, count);
        }

		inline u64 murmur_hash_64(const void* key, i32 len, u64 seed) noexcept
		{
			const u64 m = 0xc6a4a7935bd1e995ULL;
			const i32 r = 47;

			u64 h = seed ^ (len * m);

			const u64* data = (const u64*)key;
			const u64* end = data + (len / 8);

			while (data != end)
			{
#ifdef PLATFORM_BIG_ENDIAN
				u64 k = *data++;
				char* p = (char*)&k;
				char c;
				c = p[0]; p[0] = p[7]; p[7] = c;
				c = p[1]; p[1] = p[6]; p[6] = c;
				c = p[2]; p[2] = p[5]; p[5] = c;
				c = p[3]; p[3] = p[4]; p[4] = c;
#else
				u64 k = *data++;
#endif

				k *= m;
				k ^= k >> r;
				k *= m;

				h ^= k;
				h *= m;
			}

			const unsigned char* data2 = (const unsigned char*)data;

			switch (len & 7)
			{
			case 7: h ^= u64(data2[6]) << 48;
			case 6: h ^= u64(data2[5]) << 40;
			case 5: h ^= u64(data2[4]) << 32;
			case 4: h ^= u64(data2[3]) << 24;
			case 3: h ^= u64(data2[2]) << 16;
			case 2: h ^= u64(data2[1]) << 8;
			case 1: h ^= u64(data2[0]);
				h *= m;
			};

			h ^= h >> r;
			h *= m;
			h ^= h >> r;

			return h;
		}

		inline u64 murmur_hash_64_inverse(u64 h, u64 seed) noexcept
		{
			const u64 m = 0xc6a4a7935bd1e995ULL;
			const u64 minv = 0x5f7a0ea7e59b19bdULL; // Multiplicative inverse of m under % 2^64
			const i32 r = 47;

			h ^= h >> r;
			h *= minv;
			h ^= h >> r;
			h *= minv;

			u64 hforward = seed ^ (((u64)8) * m);
			u64 k = h ^ hforward;

			k *= minv;
			k ^= k >> r;
			k *= minv;

#ifdef PLATFORM_BIG_ENDIAN
			char* p = (char*)&k;
			char c;
			c = p[0]; p[0] = p[7]; p[7] = c;
			c = p[1]; p[1] = p[6]; p[6] = c;
			c = p[2]; p[2] = p[5]; p[5] = c;
			c = p[3]; p[3] = p[4]; p[4] = c;
#endif

			return k;
		}

        template<class _Ty>
        constexpr inline u32 type_hash() noexcept
        {
            constexpr auto typestr = type_name<_Ty>();
            return const_hash(typestr.data());
        }
	}
}