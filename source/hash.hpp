#pragma once

#include <standart_library.h>

namespace utl
{
	constexpr inline size_t strlen(const char* str) noexcept
	{
		size_t len = 0;
		while (str[len] != '\0')
			++len;

		return len;
	}

	constexpr inline u32 fnv1a_32_hash(const char* str, u32 len, u32 hash = 0x811c9dc5u) noexcept
	{
		for (u32 idx = 0u; idx < len; ++idx)
			hash = (hash ^ cast<u32>(str[idx])) * 0x1000193u;
		return hash;
	}

	constexpr inline u64 fnv1a_64_hash(const char* str, u64 len, u64 hash = 0xcbf29ce484222325ull) noexcept
	{
		for (u64 idx = 0ull; idx < len; ++idx)
			hash = (hash ^ cast<u64>(str[idx])) * 0x100000001b3ull;
		return hash;
	}

	inline constexpr u32 fnv1a_32_hash_cstr(const char* str, u32 hash = 0x811c9dc5u) noexcept
	{
		return fnv1a_32_hash(str, strlen(str), hash);
	}

	inline constexpr u64 fnv1a_64_hash_cstr(const char* str, u64 hash = 0xcbf29ce484222325ull) noexcept
	{
		return fnv1a_64_hash(str, strlen(str), hash);
	}



	namespace murmur32
	{
		inline constexpr u32 block(const char* p, unsigned i)
		{
			const u32 block =
				static_cast<u32>(p[0 + i * 4]) << 0 |
				static_cast<u32>(p[1 + i * 4]) << 8 |
				static_cast<u32>(p[2 + i * 4]) << 16 |
				static_cast<u32>(p[3 + i * 4]) << 24;
			return block;
		}

		inline constexpr u32 rotl32(u32 x, i8 r) 
		{
			return (x << r) | (x >> (32 - r));
		}

		inline constexpr u32 fmix32(u32 h) 
		{
			h ^= h >> 16;
			h *= 0x85ebca6b;
			h ^= h >> 13;
			h *= 0xc2b2ae35;
			h ^= h >> 16;
			return h;
		}
	}

	inline constexpr u32 murmur_hash(const char* key, u32 len, u32 seed)
	{
		constexpr const u32 c1 = 0xcc9e2d51;
		constexpr const i32 c2 = 0x1b873593;

		const u32 nblocks = len / 4u;

		u32 h1 = seed;
		
		//----------
		// body
		for (int i = 0; i < nblocks; ++i) 
		{
			uint32_t k1 = murmur32::block(key, i);

			k1 *= c1;
			k1 = murmur32::rotl32(k1, 15);
			k1 *= c2;

			h1 ^= k1;
			h1 = murmur32::rotl32(h1, 13);
			h1 = h1 * 5 + 0xe6546b64;
		}
		//----------
		// tail

		uint32_t k1 = 0;

		// The "tail" of key are the bytes that do not fit into any block,
		// len%4 is the size of the tail and 
		// 	(tail_start + i)
		// returns the i:th tail byte.
		const unsigned tail_start = len - (len % 4);
		switch (len & 3) {
		case 3: k1 ^= key[tail_start + 2] << 16;
		case 2: k1 ^= key[tail_start + 1] << 8;
		case 1: k1 ^= key[tail_start + 0];
			k1 *= c1; k1 = murmur32::rotl32(k1, 15);
			k1 *= c2; h1 ^= k1;
		};

		//----------
		// finalization

		h1 ^= len;

		h1 = murmur32::fmix32(h1);

		return h1;
	}

	namespace murmur64
	{
		inline constexpr u64 block(const char* p, u64 i)
		{
			const u64 block =
				static_cast<u64>(p[0 + i * 8]) << 0 |
				static_cast<u64>(p[1 + i * 8]) << 8 |
				static_cast<u64>(p[2 + i * 8]) << 16 |
				static_cast<u64>(p[3 + i * 8]) << 24 |
				static_cast<u64>(p[4 + i * 8]) << 32 |
				static_cast<u64>(p[5 + i * 8]) << 40 |
				static_cast<u64>(p[6 + i * 8]) << 48 |
				static_cast<u64>(p[7 + i * 8]) << 56;
			return block;
		}

		inline constexpr u64 rotl64(u64 x, i8 r)
		{
			return (x << r) | (x >> (64 - r));
		}

		inline constexpr u64 fmix64(u64 k)
		{
			k ^= k >> 33;
			k *= 0xff51afd7ed558ccdull;
			k ^= k >> 33;
			k *= 0xc4ceb9fe1a85ec53ull;
			k ^= k >> 33;

			return k;
		}
	}

	inline constexpr u64 murmur_hash_64(const char* key, i32 len, u64 seed) noexcept
	{
		constexpr const uint64_t c1 = 0x87c37b91114253d5ull;
		constexpr const uint64_t c2 = 0x4cf5ad432745937full;

		const int nblocks = len / 16;

		uint64_t h1 = seed;

		for (int i = 0; i < nblocks; i++)
		{
			uint64_t k1 = murmur64::block(key, i * 2 + 0);
			uint64_t k2 = murmur64::block(key, i * 2 + 1);

			k1 *= c1; 
			k1 = murmur64::rotl64(k1, 31); 
			k1 *= c2; 

			h1 ^= k1;
			h1 = murmur64::rotl64(h1, 27); 
			h1 = h1 * 5 + 0x52dce729;

			k2 *= c2; 
			k2 = murmur64::rotl64(k2, 33); 
			k2 *= c1; 

			h1 ^= k2;
			h1 = murmur64::rotl64(h1, 31); 
			h1 = h1 * 5 + 0x38495ab5;
		}

		uint64_t k1 = 0;
		uint64_t k2 = 0;

		const unsigned tail_start = len - (len % 16);
		const char* tail = key + tail_start;

		switch (len & 15)
		{
		case 15: k2 ^= static_cast<uint64_t>(tail[14]) << 48;
		case 14: k2 ^= static_cast<uint64_t>(tail[13]) << 40;
		case 13: k2 ^= static_cast<uint64_t>(tail[12]) << 32;
		case 12: k2 ^= static_cast<uint64_t>(tail[11]) << 24;
		case 11: k2 ^= static_cast<uint64_t>(tail[10]) << 16;
		case 10: k2 ^= static_cast<uint64_t>(tail[9]) << 8;
		case  9: k2 ^= static_cast<uint64_t>(tail[8]) << 0;
			k2 *= c2; k2 = murmur64::rotl64(k2, 33); k2 *= c1; h1 ^= k2;

		case  8: k1 ^= static_cast<uint64_t>(tail[7]) << 56;
		case  7: k1 ^= static_cast<uint64_t>(tail[6]) << 48;
		case  6: k1 ^= static_cast<uint64_t>(tail[5]) << 40;
		case  5: k1 ^= static_cast<uint64_t>(tail[4]) << 32;
		case  4: k1 ^= static_cast<uint64_t>(tail[3]) << 24;
		case  3: k1 ^= static_cast<uint64_t>(tail[2]) << 16;
		case  2: k1 ^= static_cast<uint64_t>(tail[1]) << 8;
		case  1: k1 ^= static_cast<uint64_t>(tail[0]) << 0;
			k1 *= c1; k1 = murmur64::rotl64(k1, 31); k1 *= c2; h1 ^= k1;
		};

		//----------
		// finalization

		h1 ^= len;

		h1 = murmur64::fmix64(h1);

		return h1;
	}

	inline constexpr u32 operator "" _fnv1a32(char const* s, size_t count) noexcept
	{
		return fnv1a_32_hash(s, count);
	}

	inline constexpr u64 operator "" _fnv1a64(char const* s, size_t count) noexcept
	{
		return fnv1a_64_hash(s, count);
	}

	inline constexpr u32 operator "" _mr32(char const* s, size_t count) noexcept
	{
		return murmur_hash(s, count, 0);
	}

	inline constexpr u32 operator "" _mr64(char const* s, size_t count) noexcept
	{
		return murmur_hash_64(s, count, 0);
	}


	template<class _Ty>
	struct hash64
	{
		u64 operator()(const _Ty& data) const
		{
			return murmur_hash_64(reinterpret_cast<const char*>(&data), sizeof(_Ty), 0ull);
		}
	};

	
	template<typename _Ty, typename _Traits, typename _Allocator>
	struct hash64<std::basic_string<_Ty, _Traits, _Allocator>>
	{
		u64 operator()(const std::basic_string<_Ty, _Traits, _Allocator>& data) const
		{
			return murmur_hash_64(reinterpret_cast<const char*>(data.c_str()), data.size() * sizeof(_Ty), 0ull);
		}
	};

	//template<>
	//struct hash<std::basic_string<char, std::char_traits<char>, std::allocator<char>>>
	//{
	//	u64 operator()(const std::basic_string<char, std::char_traits<char>, std::allocator<char>>& data) const
	//	{
	//		return fnv1a_64_hash(data.c_str(), data.size());
	//	}
	//};
}