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
		const u32 tail_start = len - (len % 4);
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

	inline constexpr u32 murmur_hash(const char* str, u32 seed = 0u)
	{
		return murmur_hash(str, strlen(str), seed);
	}

	template<typename _Ty, typename _Traits = std::char_traits<_Ty>, typename _Allocator = std::allocator<_Ty>>
	inline u32 murmur_hash(const std::basic_string<_Ty, _Traits, _Allocator>& str, u32 seed = 0u)
	{
		return murmur_hash(str.c_str(), str.size(), seed);
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

	inline constexpr u64 murmur_hash_64(const char* key, u64 len, u64 seed) noexcept
	{
		constexpr const u64 c1 = 0x87c37b91114253d5ULL;
		constexpr const u64 c2 = 0x4cf5ad432745937fULL;

		u64 h1 = seed;

		const u64 nblocks = len / 16ull;

		for (u64 i = 0; i < nblocks; ++i)
		{
			u64 k1 = murmur64::block(key, i * 2);
			u64 k2 = murmur64::block(key, i * 2 + 1);

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

		const u64 tail_start = nblocks * 16ull;
		u64 k1 = 0;
		u64 k2 = 0;

		switch (len & 15) {
		case 15: k2 ^= u64(key[tail_start + 14]) << 48;
		case 14: k2 ^= u64(key[tail_start + 13]) << 40;
		case 13: k2 ^= u64(key[tail_start + 12]) << 32;
		case 12: k2 ^= u64(key[tail_start + 11]) << 24;
		case 11: k2 ^= u64(key[tail_start + 10]) << 16;
		case 10: k2 ^= u64(key[tail_start + 9]) << 8;
		case 9:  k2 ^= u64(key[tail_start + 8]);
			k2 *= c2;
			k2 = murmur64::rotl64(k2, 33);
			k2 *= c1;
			h1 ^= k2;

		case 8:  k1 ^= u64(key[tail_start + 7]) << 56;
		case 7:  k1 ^= u64(key[tail_start + 6]) << 48;
		case 6:  k1 ^= u64(key[tail_start + 5]) << 40;
		case 5:  k1 ^= u64(key[tail_start + 4]) << 32;
		case 4:  k1 ^= u64(key[tail_start + 3]) << 24;
		case 3:  k1 ^= u64(key[tail_start + 2]) << 16;
		case 2:  k1 ^= u64(key[tail_start + 1]) << 8;
		case 1:  k1 ^= u64(key[tail_start + 0]);
			k1 *= c1;
			k1 = murmur64::rotl64(k1, 31);
			k1 *= c2;
			h1 ^= k1;
		}

		h1 ^= len;
		h1 = murmur64::fmix64(h1);

		return h1;
	}

	inline constexpr u64 murmur_hash_64(const char* str, u64 seed = 0ull)
	{
		return murmur_hash_64(str, strlen(str), seed);
	}

	template<typename _Ty, typename _Traits = std::char_traits<_Ty>, typename _Allocator = std::allocator<_Ty>>
	inline u64 murmur_hash_64(const std::basic_string<_Ty, _Traits, _Allocator>& str, u64 seed = 0ull)
	{
		return murmur_hash_64(str.c_str(), str.size(), seed);
	}

	inline constexpr u32 operator "" _hash32(char const* s, size_t count) noexcept
	{
		return murmur_hash(s, count, 0);
	}

	inline constexpr u64 operator "" _hash64(char const* s, size_t count) noexcept
	{
		return murmur_hash_64(s, count, 0);
	}


	template<class _Ty>
	struct hash32
	{
		u32 operator()(const _Ty& data) const
		{
			return murmur_hash(reinterpret_cast<const char*>(&data), sizeof(_Ty), 0u);
		}
	};


	template<typename _Ty, typename _Traits, typename _Allocator>
	struct hash32<std::basic_string<_Ty, _Traits, _Allocator>>
	{
		u32 operator()(const std::basic_string<_Ty, _Traits, _Allocator>& data) const
		{
			return murmur_hash(reinterpret_cast<const char*>(data.c_str()), data.size() * sizeof(_Ty), 0u);
		}
	};

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


	// slot map impl
	template<typename _Ty, typename _HashType>
	class slot_map
	{
	public:
	private:
	};
}