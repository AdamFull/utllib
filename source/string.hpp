#pragma once

#include <standart_library.h>
#include <utils/hash.h>

namespace utl
{
	// String conversion
	template<typename _Ty, typename _Traits = std::char_traits<_Ty>, typename _Allocator = std::allocator<_Ty>>
	inline std::string to_utf8(const std::basic_string<_Ty, _Traits, _Allocator>& str) {}

	template<>
	inline std::string to_utf8<wchar_t>(const std::wstring& str)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		return converter.to_bytes(str);
	}

	template<>
	inline std::string to_utf8<char16_t>(const std::u16string& str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
		return converter.to_bytes(str);
	}

	template<>
	inline std::string to_utf8<char32_t>(const std::u32string& str)
	{
		std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
		return converter.to_bytes(str);
	}

	template<typename _Ty, typename _Traits = std::char_traits<_Ty>, typename _Allocator = std::allocator<_Ty>>
	inline std::basic_string<_Ty, _Traits, _Allocator> from_utf8(const std::string& str) {}

	template<>
	inline std::wstring from_utf8<wchar_t>(const std::string& str)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		return converter.from_bytes(str);
	}

	template<>
	inline std::u16string from_utf8<char16_t>(const std::string& str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
		return converter.from_bytes(str);
	}

	template<>
	inline std::u32string from_utf8<char32_t>(const std::string& str)
	{
		std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
		return converter.from_bytes(str);
	}

	namespace base64_detail
	{
		constexpr std::string_view base64_chars =
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz"
			"0123456789+/";
	}

	inline bool is_base64(unsigned char c) 
	{
		return (isalnum(c) || (c == '+') || (c == '/'));
	}

	inline std::string to_base64(const u8* data, size_t size)
	{
		std::string ret;
		int i = 0;
		int j = 0;
		uint8_t char_array_3[3];
		uint8_t char_array_4[4];

		while (size--)
		{
			char_array_3[i++] = *(data++);
			if (i == 3)
			{
				char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
				char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
				char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
				char_array_4[3] = char_array_3[2] & 0x3f;

				for (i = 0; (i < 4); i++)
					ret += base64_detail::base64_chars[char_array_4[i]];
				i = 0;
			}
		}

		if (i)
		{
			for (j = i; j < 3; j++)
				char_array_3[j] = '\0';

			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (j = 0; (j < i + 1); j++)
				ret += base64_detail::base64_chars[char_array_4[j]];

			while ((i++ < 3))
				ret += '=';
		}

		return ret;
	}

	inline utl::vector<u8> from_base64(const std::string& data)
	{
		int in_len = data.size();
		int i = 0;
		int j = 0;
		int in_ = 0;
		uint8_t char_array_4[4], char_array_3[3];
		utl::vector<uint8_t> ret;

		while (in_len-- && (data[in_] != '=') && is_base64(data[in_]))
		{
			char_array_4[i++] = data[in_];
			in_++;
			if (i == 4)
			{
				for (i = 0; i < 4; i++)
					char_array_4[i] = base64_detail::base64_chars.find(char_array_4[i]);

				char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
				char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
				char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

				for (i = 0; (i < 3); i++)
					ret.push_back(char_array_3[i]);
				i = 0;
			}
		}

		if (i)
		{
			for (j = i; j < 4; j++)
				char_array_4[j] = 0;

			for (j = 0; j < 4; j++)
				char_array_4[j] = base64_detail::base64_chars.find(char_array_4[j]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (j = 0; (j < i - 1); j++)
				ret.push_back(char_array_3[j]);
		}

		return ret;
	}

	namespace impl
	{
		/// Base declaration of our constexpr string_view concatenation helper
		template <std::string_view const&, typename, std::string_view const&, typename>
		struct concat;

		/// Specialisation to yield indices for each char in both provided string_views,
		/// allows us flatten them into a single char array
		template <std::string_view const& S1,
			std::size_t... I1,
			std::string_view const& S2,
			std::size_t... I2>
		struct concat<S1, std::index_sequence<I1...>, S2, std::index_sequence<I2...>>
		{
			static constexpr const char value[]{ S1[I1]..., S2[I2]..., 0 };
		};
	} // namespace impl

	/// Base definition for compile time joining of strings
	template <std::string_view const&...> struct join;

	/// When no strings are given, provide an empty literal
	template <>
	struct join<>
	{
		static constexpr std::string_view value = "";
	};

	/// Base case for recursion where we reach a pair of strings, we concatenate
	/// them to produce a new constexpr string
	template <std::string_view const& S1, std::string_view const& S2>
	struct join<S1, S2>
	{
		static constexpr std::string_view value =
			impl::concat<S1,
			std::make_index_sequence<S1.size()>,
			S2,
			std::make_index_sequence<S2.size()>>::value;
	};

	/// Main recursive definition for constexpr joining, pass the tail down to our
	/// base case specialisation
	template <std::string_view const& S, std::string_view const&... Rest>
	struct join<S, Rest...>
	{
		static constexpr std::string_view value =
			join<S, join<Rest...>::value>::value;
	};

	/// Join constexpr string_views to produce another constexpr string_view
	template <std::string_view const&... Strs>
	static constexpr auto join_v = join<Strs...>::value;


	template<typename _Ty, typename _HashType>
	class id_string
	{
	public:
		using value_type = std::basic_string<_Ty, std::char_traits<_Ty>, std::allocator<_Ty>>;
		using hash_type = _HashType;

		id_string() : _hash(0) {}

		id_string(const value_type& str)
			: _hash(compute_hash(str))
		{
			_store_string(_hash, str);
		}

		id_string(const value_type& str, hash_type hash)
			: _hash(hash)
		{
			_store_string(_hash, str);
		}

		// Copy constructor
		id_string(const id_string& other)
			: _hash(other._hash)
		{
		}

		// Assignment operator
		id_string& operator=(const id_string& other)
		{
			if (this != &other && _hash != other._hash && _hash != 0)
			{
				_hash = other._hash;
				_store_string(_hash, other.c_str());
			}
			return *this;
		}

		const _Ty* c_str() const
		{
			auto it = _string_pool().find(_hash);
			if (it != _string_pool().end())
				return it->second.c_str();

			return nullptr;
		}

		const value_type str() const
		{
			static value_type empty{ "" };

			auto it = _string_pool().find(_hash);
			if (it != _string_pool().end())
				return it->second;

			return empty;
		}

		hash_type hash() const 
		{
			return _hash;
		}

		bool valid() const
		{
			return _hash != 0;
		}

	private:
		hash_type _hash;

		static hash_type compute_hash(const value_type& str)
		{
			if constexpr (sizeof(hash_type) == sizeof(u32))
			{
				return murmur_hash((char*)str.data(), str.size() * sizeof(_Ty), 0);
			}
			else if constexpr (sizeof(hash_type) == sizeof(u64))
			{
				return murmur_hash_64((char*)str.data(), str.size() * sizeof(_Ty), 0);
			}
		}

		static void _store_string(hash_type hash, const value_type& str)
		{
			_string_pool()[hash] = str;
		}

		static utl::hash_map<hash_type, std::string>& _string_pool()
		{
			static utl::hash_map<hash_type, value_type> pool;
			return pool;
		}
	};

	using id_string32 = id_string<char, u32>;
	using id_string64 = id_string<char, u64>;

	using id_wstring32 = id_string<wchar_t, u32>;
	using id_wstring64 = id_string<wchar_t, u64>;

	using id_u16string32 = id_string<char16_t, u32>;
	using id_u16string64 = id_string<char16_t, u64>;

	using id_u32string32 = id_string<char32_t, u32>;
	using id_u32string64 = id_string<char32_t, u64>;
}