#pragma once

#include "reflection.h"

namespace utl
{
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

	// Murmurhash is from this blogspot: https://bitsquid.blogspot.com/2011/08/code-snippet-murmur-hash-inverse-pre.html
	inline u32 murmur_hash(const void* key, i32 len, u32 seed)
	{
		// 'm' and 'r' are mixing constants generated offline.
		// They're not really 'magic', they just happen to work well.

		const u32 m = 0x5bd1e995;
		const i32 r = 24;

		// Initialize the hash to a 'random' value

		u32 h = seed ^ len;

		// Mix 4 bytes at a time into the hash

		const u8* data = (const u8*)key;

		while (len >= 4)
		{
#ifdef PLATFORM_BIG_ENDIAN
			u32 k = (data[0]) + (data[1] << 8) + (data[2] << 16) + (data[3] << 24);
#else
			u32 k = *(u32*)data;
#endif

			k *= m;
			k ^= k >> r;
			k *= m;

			h *= m;
			h ^= k;

			data += 4;
			len -= 4;
		}

		// Handle the last few bytes of the input array

		switch (len)
		{
		case 3: h ^= data[2] << 16;
		case 2: h ^= data[1] << 8;
		case 1: h ^= data[0];
			h *= m;
		};

		// Do a few final mixes of the hash to ensure the last few
		// bytes are well-incorporated.

		h ^= h >> 13;
		h *= m;
		h ^= h >> 15;

		return h;
	}

	/// Inverts a (h ^= h >> s) operation with 8 <= s <= 16
	inline u32 invert_shift_xor(u32 hs, u32 s)
	{
		assert(s >= 8 && s <= 16);
		u32 hs0 = hs >> 24;
		u32 hs1 = (hs >> 16) & 0xff;
		u32 hs2 = (hs >> 8) & 0xff;
		u32 hs3 = hs & 0xff;

		u32 h0 = hs0;
		u32 h1 = hs1 ^ (h0 >> (s - 8));
		u32 h2 = (hs2 ^ (h0 << (16 - s)) ^ (h1 >> (s - 8))) & 0xff;
		u32 h3 = (hs3 ^ (h1 << (16 - s)) ^ (h2 >> (s - 8))) & 0xff;
		return (h0 << 24) + (h1 << 16) + (h2 << 8) + h3;
	}

	inline u32 murmur_hash_inverse(u32 h, u32 seed)
	{
		const u32 m = 0x5bd1e995;
		const u32 minv = 0xe59b19bd;	// Multiplicative inverse of m under % 2^32
		const i32 r = 24;

		h = invert_shift_xor(h, 15);
		h *= minv;
		h = invert_shift_xor(h, 13);

		u32 hforward = seed ^ 4;
		hforward *= m;
		u32 k = hforward ^ h;
		k *= minv;
		k ^= k >> r;
		k *= minv;

#ifdef PLATFORM_BIG_ENDIAN
		char* data = (char*)&k;
		k = (data[0]) + (data[1] << 8) + (data[2] << 16) + (data[3] << 24);
#endif

		return k;
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

		const u8* data2 = (const u8*)data;

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