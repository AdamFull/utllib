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

	constexpr inline const u32 packUint16x2(u16 lhs, u16 rhs)
	{
		return (rhs << 16) | (lhs & 0xFFFF);
	}

	inline const void unpackUint16x2(u32 packed, u16& lhs, u16& rhs)
	{
		rhs = cast<u16>(packed >> 16);
		lhs = cast<u16>(packed & 0xFFFF);
	}

	template<class _Ty>
	inline void safe_delete(_Ty*& ptr)
	{
		if(ptr)
		{
			delete ptr;
			ptr = nullptr;
		}
	}

	template<class _Ty>
	inline void safe_delete(const std::allocator<_Ty>& alloc, _Ty*& ptr)
	{
		if(ptr)
		{
			alloc.destroy(ptr);
        	alloc.deallocate(ptr, 1);
        	ptr = nullptr;
		}
	}

	template<class _Ty>
	inline void safe_delete_array(_Ty*& ptr)
	{
		if(ptr)
		{
			delete[] ptr;
			ptr = nullptr;
		}
	}

	template<class _Ty>
	inline void safe_delete_array(const std::allocator<_Ty>& alloc, _Ty*& ptr, size_t count)
	{
		if(ptr)
		{
			alloc.destroy(ptr);
        	alloc.deallocate(ptr, count);
        	ptr = nullptr;
		}
	}
}