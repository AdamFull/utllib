#pragma once

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <typeindex>

#include <cassert>

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
#include <codecvt>
#include <locale> 
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

	// U16 packing
	constexpr inline const uint16_t pack_u16_8x2(uint8_t lhs, uint8_t rhs)
	{
		return (rhs << 8) | (lhs & 0xFF);
	}
	
	inline const void unpack_u16_8x2(uint16_t packed, uint8_t& lhs, uint8_t& rhs)
	{
		rhs = cast<uint8_t>(packed >> 8);
		lhs = cast<uint8_t>(packed & 0xFF);
	}
	
	// U32 packing
	constexpr inline const uint32_t pack_u32_16x2(uint16_t lhs, uint16_t rhs)
	{
		return (rhs << 16) | (lhs & 0xFFFF);
	}
	
	inline const void unpack_u32_16x2(uint32_t packed, uint16_t& lhs, uint16_t& rhs)
	{
		rhs = cast<uint16_t>(packed >> 16);
		lhs = cast<uint16_t>(packed & 0xFFFF);
	}
	
	//constexpr inline const uint32_t pack_u32_8x4(uint8_t l0, uint8_t l1, uint8_t r0, uint8_t r1)
	//{
	//	return (l0 << 24) + (l1 << 16) + (r0 << 8) + r1;
	//}
	//
	//inline const void unpack_u32_8x4(uint32_t packed, uint8_t& l0, uint8_t& l1, uint8_t& r0, uint8_t& r1)
	//{
	//	l0 = (packed & 0xff000000) >> 24;
	//	l1 = (packed & 0x00ff0000) >> 16;
	//	r0 = (packed & 0x0000ff00) >> 8;
	//	r1 = (packed & 0x000000ff);
	//}

	// U64 packing
	//constexpr inline const uint64_t pack_u64_8x8(uint8_t l0, uint8_t l1, uint8_t l2, uint8_t l3, uint8_t r0, uint8_t r1, uint8_t r2, uint8_t r3)
	//{
	//	return (l1 << 56ull) + (l1 << 48ull) + (l2 << 40ull) + (l3 << 32ull) + (r0 << 24ull) + (r1 << 16ull) + (r2 << 8ull) + r3;
	//}
	//
	//inline const void unpack_u64_8x8(uint64_t packed, uint8_t& l0, uint8_t& l1, uint8_t& l2, uint8_t& l3, uint8_t& r0, uint8_t& r1, uint8_t& r2, uint8_t& r3)
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
	//constexpr inline const uint64_t pack_u64_16x4(uint16_t l0, uint16_t l1, uint16_t r0, uint16_t r1)
	//{
	//	return (l0 << 48ull) + (l1 << 32ull) + (r0 << 16ull) + r1;
	//}
	//
	//inline const void unpack_u64_16x4(uint64_t packed, uint16_t& l0, uint16_t& l1, uint16_t& r0, uint16_t& r1)
	//{
	//	l0 = (packed & 0xffff000000000000ull) >> 48ull;
	//	l1 = (packed & 0x0000ffff00000000ull) >> 32ull;
	//	r0 = (packed & 0x00000000ffff0000ull) >> 16ull;
	//	r1 = (packed & 0x000000000000ffffull);
	//}
	//
	//constexpr inline const uint64_t pack_u64_32x2(uint32_t lhs, uint32_t rhs)
	//{
	//	return (lhs << 32ull) + rhs;
	//}
	//
	//inline const void unpack_u64_32x2(uint64_t packed, uint32_t& lhs, uint32_t& rhs)
	//{
	//	lhs = (packed & 0xffffffff00000000ull) >> 32ull;
	//	rhs = (packed & 0x00000000ffffffffull);
	//}

	template<class _Kty, class _Ty,
		class _Hasher = std::hash<_Kty>,
		class _Keyeq = std::equal_to<_Kty>,
		class _Alloc = std::allocator<std::pair<const _Kty, _Ty>>>
	using hash_map = std::unordered_map<_Kty, _Ty, _Hasher, _Keyeq, _Alloc>;

	template<class _Ty, class _Alloc = std::allocator<_Ty>>
	using vector = std::vector<_Ty, _Alloc>;

	template<class _Elem>
	using basic_string = std::basic_string<_Elem, std::char_traits<_Elem>, std::allocator<_Elem>>;
	
	using string = basic_string<char>;
	using wstring = basic_string<wchar_t>;
	using u16string = basic_string<char16_t>;
	using u32string = basic_string<char32_t>;

	using stringstream = std::basic_stringstream<char, std::char_traits<char>, std::allocator<char>>;
	using wstringstream = std::basic_stringstream<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>;

	using istringstream = std::basic_istringstream<char, std::char_traits<char>, std::allocator<char>>;
	using wistringstream = std::basic_istringstream<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>;
	using ostringstream = std::basic_ostringstream<char, std::char_traits<char>, std::allocator<char>>;
	using wostringstream = std::basic_ostringstream<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>;

	template<class _Codecvt, class _Elem = wchar_t>
	using wstring_convert = std::wstring_convert<_Codecvt, _Elem, std::allocator<_Elem>, std::allocator<char>>;

	inline string make_string(const std::string& str) { return string(str.begin(), str.end()); }
	inline string make_string(const std::filesystem::path& path) 
	{ 
		auto str = path.string(); 
		return string(str.begin(), str.end()); 
	}

	inline u16string make_u16string(const std::u16string& str) { return u16string(str.begin(), str.end()); }
	inline u16string make_u16string(const std::filesystem::path& path)
	{
		auto str = path.u16string();
		return u16string(str.begin(), str.end());
	}

	inline u32string make_u32string(const std::u32string& str) { return u32string(str.begin(), str.end()); }
	inline u32string make_u32string(const std::filesystem::path& path)
	{
		auto str = path.u32string();
		return u32string(str.begin(), str.end());
	}

	inline wstring make_wstring(const std::wstring& str) { return wstring(str.begin(), str.end()); }
	inline wstring make_wstring(const std::filesystem::path& path)
	{
		auto str = path.wstring();
		return wstring(str.begin(), str.end());
	}

	[[nodiscard]] inline long long stoll(const string& _Str, size_t* _Idx = nullptr, int _Base = 10) 
	{
		int& _Errno_ref = errno;
		const char* _Ptr = _Str.c_str();
		char* _Eptr;
		_Errno_ref = 0;
		const long long _Ans = std::strtoll(_Ptr, &_Eptr, _Base);

		if (_Ptr == _Eptr) {
			std::invalid_argument("invalid stoll argument");
		}

		if (_Errno_ref == ERANGE)
			std::out_of_range("stoll argument out of range");

		if (_Idx)
			*_Idx = static_cast<size_t>(_Eptr - _Ptr);

		return _Ans;
	}

	[[nodiscard]] inline double stod(const string& _Str, size_t* _Idx = nullptr) 
	{
		int& _Errno_ref = errno;
		const char* _Ptr = _Str.c_str();
		char* _Eptr;
		_Errno_ref = 0;
		const double _Ans = std::strtod(_Ptr, &_Eptr);

		if (_Ptr == _Eptr)
			std::invalid_argument("invalid stod argument");

		if (_Errno_ref == ERANGE)
			std::out_of_range("stod argument out of range");

		if (_Idx)
			*_Idx = static_cast<size_t>(_Eptr - _Ptr);

		return _Ans;
	}

	//template<class _Elem>
	//[[nodiscard]] inline basic_string<_Elem> vformat(const std::string_view _Fmt, const std::format_args _Args)
	//{
	//	basic_string<_Elem> _Str;
	//	_Str.reserve(_Fmt.size() + _Args._Estimate_required_capacity());
	//	std::vformat_to(std::back_insert_iterator{ _Str }, _Fmt, _Args);
	//	return _Str;
	//}
	//
	//template<class _Elem>
	//[[nodiscard]] basic_string<_Elem> vformat(const std::locale& _Loc, const std::string_view _Fmt, const std::format_args _Args)
	//{
	//	basic_string<_Elem> _Str;
	//	_Str.reserve(_Fmt.size() + _Args._Estimate_required_capacity());
	//	std::vformat_to(std::back_insert_iterator{ _Str }, _Loc, _Fmt, _Args);
	//	return _Str;
	//}
	//
	//template <class... _Types>
	//[[nodiscard]] inline string format(const std::format_string<_Types...> _Fmt, _Types&&... _Args)
	//{
	//	return vformat<char>(_Fmt.get(), std::make_format_args(_Args...));
	//}
	//
	//template <class... _Types>
	//[[nodiscard]] inline wstring format(const std::wformat_string<_Types...> _Fmt, _Types&&... _Args)
	//{
	//	return vformat<wchar_t>(_Fmt.get(), std::make_wformat_args(_Args...));
	//}
	//
	//template <class... _Types>
	//[[nodiscard]] inline string format(const std::locale& _Loc, const std::format_string<_Types...> _Fmt, _Types&&... _Args)
	//{
	//	return vformat<char>(_Loc, _Fmt.get(), std::make_format_args(_Args...));
	//}
	//
	//template <class... _Types>
	//[[nodiscard]] inline wstring format(const std::locale& _Loc, const std::wformat_string<_Types...> _Fmt, _Types&&... _Args)
	//{
	//	return vformat<wchar_t>(_Loc, _Fmt.get(), std::make_wformat_args(_Args...));
	//}
}