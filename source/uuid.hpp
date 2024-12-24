#pragma once

#include <standart_library.h>

// all constexpr part just copy from here: https://gist.github.com/AlexBAV/b58e92d7632bae5d6f5947be455f796f
namespace utl
{
	struct guid
	{
		uint32_t d1;
		uint16_t d2;
		uint16_t d3;
		uint8_t d4[8];
	};

	namespace details
	{
		constexpr const uint64_t short_guid_form_length = 36ull;	// XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX
		constexpr const uint64_t long_guid_form_length = 38ull;	// {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX}

		constexpr int32_t parse_hex_digit(const char c)
		{
			using namespace std::string_literals;

			if ('0' <= c && c <= '9')
				return c - '0';
			else if ('a' <= c && c <= 'f')
				return 10 + c - 'a';
			else if ('A' <= c && c <= 'F')
				return 10 + c - 'A';
			else
				throw std::domain_error{ "invalid character in GUID"s };
		}

		template<typename _Ty>
		constexpr _Ty parse_hex(const char* ptr)
		{
			constexpr const uint64_t digits = sizeof(_Ty) * 2ull;
			_Ty result{};
			for (uint64_t i = 0; i < digits; ++i)
				result |= parse_hex_digit(ptr[i]) << (4ull * (digits - i - 1ull));

			return result;
		}

		constexpr guid make_guid_helper(const char* begin)
		{
			guid result{};
			result.d1 = parse_hex<uint32_t>(begin);
			begin += 8ull + 1ull;
			result.d2 = parse_hex<uint16_t>(begin);
			begin += 4ull + 1ull;
			result.d3 = parse_hex<uint16_t>(begin);
			begin += 4ull + 1ull;
			result.d4[0] = parse_hex<uint8_t>(begin);
			begin += 2ull;
			result.d4[1] = parse_hex<uint8_t>(begin);
			begin += 2ull + 1ull;
			for (uint64_t i = 0ull; i < 6ull; ++i)
				result.d4[i + 2] = parse_hex<uint8_t>(begin + i * 2ull);

			return result;
		}

		template<uint64_t _N>
		constexpr guid make_guid(const char(&str)[_N])
		{
			using namespace std::string_literals;
			static_assert(_N == (long_guid_form_length + 1ull) || _N == (short_guid_form_length + 1ull), "String GUID of the form {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX} or XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX is expected");

			if constexpr (_N == (long_guid_form_length + 1ull))
			{
				if (str[0ull] != '{' || str[long_guid_form_length - 1ull] != '}')
					throw std::domain_error{ "Missing opening or closing brace"s };
			}

			return make_guid_helper(str + (_N == (long_guid_form_length + 1ull) ? 1ull : 0ull));
		}
	}

	std::string to_string(const guid& id);
	guid generate_guid();

	inline constexpr guid operator "" _guid(const char* str, uint64_t N)
	{
		using namespace std::string_literals;
		using namespace details;

		if (!(N == long_guid_form_length || N == short_guid_form_length))
			throw std::domain_error{ "String GUID of the form {XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX} or XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX is expected"s };
		if (N == long_guid_form_length && (str[0] != '{' || str[long_guid_form_length - 1] != '}'))
			throw std::domain_error{ "Missing opening or closing brace"s };

		return make_guid_helper(str + (N == long_guid_form_length ? 1 : 0));
	}
}