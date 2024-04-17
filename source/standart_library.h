#pragma once

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <typeindex>

#include <cassert>

//#include <mimalloc.h>

using i8 = int8_t;
using u8 = uint8_t;
using i16 = int16_t;
using u16 = uint16_t;
using i32 = int32_t;
using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;
using f32 = float;
using f64 = double;

#include <algorithm>
#include <any>
#include <array>
#include <atomic>
#include <bit>
#include <bitset>
#include <chrono>
#include <deque>
#include <forward_list>
#include <functional>
#include <initializer_list>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <limits>
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <stack>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <valarray>
#include <variant>
#include <vector>
#include <iterator>

#include <exception>
#include <stdexcept>
#include <filesystem>

#include <span>
#include <ranges>
#include <format>

namespace utl
{
	template<class _Ty, class _KTy>
	inline constexpr _Ty cast(_KTy object)
	{
		return static_cast<_Ty>(object);
	}

	template<class _Ty, class _ReturnType = typename std::underlying_type<_Ty>::type>
	inline constexpr _ReturnType enum_cast(_Ty enum_value)
	{
		return cast<_ReturnType>(enum_value);
	}

	// U16 packing
	constexpr inline const u16 pack_u16_8x2(u8 lhs, u8 rhs)
	{
		return (rhs << 8) | (lhs & 0xFF);
	}

	inline const void unpack_u32_16x2(u16 packed, u8& lhs, u8& rhs)
	{
		rhs = cast<u8>(packed >> 8);
		lhs = cast<u8>(packed & 0xFF);
	}

	// U32 packing
	constexpr inline const u32 pack_u32_16x2(u16 lhs, u16 rhs)
	{
		return (rhs << 16) | (lhs & 0xFFFF);
	}

	inline const void unpack_u32_16x2(u32 packed, u16& lhs, u16& rhs)
	{
		rhs = cast<u16>(packed >> 16);
		lhs = cast<u16>(packed & 0xFFFF);
	}

	constexpr inline const u32 pack_u32_8x4(u8 l0, u8 l1, u8 r0, u8 r1)
	{
		return (l0 << 24) + (l1 << 16) + (r0 << 8) + r1;
	}

	inline const void unpack_u32_8x4(u32 packed, u8& l0, u8& l1, u8& r0, u8& r1)
	{
		l0 = (packed & 0xff000000) >> 24;
		l1 = (packed & 0x00ff0000) >> 16;
		r0 = (packed & 0x0000ff00) >> 8;
		r1 = (packed & 0x000000ff);
	}

	// U64 packing
	//constexpr inline const u64 pack_u64_8x8(u8 l0, u8 l1, u8 l2, u8 l3, u8 r0, u8 r1, u8 r2, u8 r3)
	//{
	//	return (l1 << 56ull) + (l1 << 48ull) + (l2 << 40ull) + (l3 << 32ull) + (r0 << 24ull) + (r1 << 16ull) + (r2 << 8ull) + r3;
	//}
	//
	//inline const void unpack_u64_8x8(u64 packed, u8& l0, u8& l1, u8& l2, u8& l3, u8& r0, u8& r1, u8& r2, u8& r3)
	//{
	//	l0 = (packed & 0xff00000000000000ull) >> 56ull;
	//	l1 = (packed & 0x00ff000000000000ull) >> 48ull;
	//	l2 = (packed & 0x0000ff0000000000ull) >> 40ull;
	//	l3 = (packed & 0x000000ff00000000ull) >> 32ull;
	//	r0 = (packed & 0x00000000ff000000ull) >> 24ull;
	//	r1 = (packed & 0x0000000000ff0000ull) >> 16ull;
	//	r2 = (packed & 0x000000000000ff00ull) >> 8ull;
	//	r3 = (packed & 0x00000000000000ffull);
	//}
	//
	//constexpr inline const u64 pack_u64_16x4(u16 l0, u16 l1, u16 r0, u16 r1)
	//{
	//	return (l0 << 48ull) + (l1 << 32ull) + (r0 << 16ull) + r1;
	//}
	//
	//inline const void unpack_u64_16x4(u64 packed, u16& l0, u16& l1, u16& r0, u16& r1)
	//{
	//	l0 = (packed & 0xffff000000000000ull) >> 48ull;
	//	l1 = (packed & 0x0000ffff00000000ull) >> 32ull;
	//	r0 = (packed & 0x00000000ffff0000ull) >> 16ull;
	//	r1 = (packed & 0x000000000000ffffull);
	//}
	//
	//constexpr inline const u64 pack_u64_32x2(u32 lhs, u32 rhs)
	//{
	//	return (lhs << 32ull) + rhs;
	//}
	//
	//inline const void unpack_u64_32x2(u64 packed, u32& lhs, u32& rhs)
	//{
	//	lhs = (packed & 0xffffffff00000000ull) >> 32ull;
	//	rhs = (packed & 0x00000000ffffffffull);
	//}

	template<class _Ty>
	inline void release_ptr(const std::allocator<_Ty>& alloc, _Ty*& ptr, size_t count = 1ull)
	{
		if(ptr)
		{
			alloc.destroy(ptr);
        	alloc.deallocate(ptr, count);
        	ptr = nullptr;
		}
	}

	template<class _Ty>
	inline void release_ptr(_Ty*& ptr)
	{
		if(ptr)
		{
			delete ptr;
			ptr = nullptr;
		}
	}

	template<class _Ty>
	inline void release_arr(_Ty*& ptr)
	{
		if(ptr)
		{
			delete[] ptr;
			ptr = nullptr;
		}
	}
}