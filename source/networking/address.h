#pragma once

#include <standart_library.h>

// Based on: https://github.com/gdelugre/literal_ipaddr/blob/master/ipaddr.h

namespace utl
{
	namespace networking
	{
		namespace parsing
		{
			template<u64 _Size>
			inline constexpr i64 find_char(const char(&str)[_Size], u64 from, char c)
			{
				for (i64 idx = from; idx < _Size; idx++)
				{
					if (str[idx] == c)
						return idx;
				}
				return -1;
			}

			template<u64 _Size>
			inline constexpr i64 rfind_char(const char(&str)[_Size], char c) 
			{
				for (i64 idx = _Size - 1; idx >= 0; idx--) 
				{
					if (str[idx] == c)
						return idx;
				}
				return -1;
			}

			inline constexpr bool isdigit(char c)
			{
				return c >= '0' && c <= '9';
			}

			inline constexpr bool isxdigit(char c)
			{
				return isdigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
			}

			inline constexpr char islower(char c)
			{
				return (c >= 'a' && c <= 'z');
			}

			inline constexpr char toupper(char c)
			{
				if (!islower(c))
					return c;

				return c ^ 0x20;
			}

			template <i32 base>
			inline constexpr bool is_valid_digit(i8 c)
			{
				static_assert(base == 8 || base == 10 || base == 16, "Invalid base parameter");

				if constexpr (base == 8)
					return (c >= '0' && c <= '7');
				else if constexpr (base == 10)
					return isdigit(c);
				else if constexpr (base == 16)
					return isxdigit(c);
			}

			template <i32 base>
			inline constexpr i32 convert_digit(i8 c)
			{
				static_assert(base == 8 || base == 10 || base == 16, "Invalid base parameter");

				if (!is_valid_digit<base>(c))
					return -1;

				if constexpr (base == 8 || base == 10) {
					return c - '0';
				}
				else if constexpr (base == 16)
				{
					if (isdigit(c))
						return convert_digit<10>(c);
					else if (c >= 'A' && c <= 'F')
						return c - 'A' + 10;
					else
						return c - 'a' + 10;
				}
			}

			template <i32 base, i8 sep, u32 max_value, u64 max_length = 0ull, u64 _Size>
			inline constexpr i64 parse_address_component(const char(&str)[_Size], u64 idx)
			{
				i64 res{ 0ll };

				if (_Size - 1 - idx <= 0ull || str[idx] == sep)
					return -1ll;

				for (u64 i = idx; i < _Size - 1ull && str[i] != sep; i++)
				{
					if (max_length > 0ull && (i - idx + 1ull) > max_length)
						return -1ll;

					if (!is_valid_digit<base>(str[i]))
						return -1ll;

					res *= base;
					res += convert_digit<base>(str[i]);

					if (res > max_value)
						return -1ll;
				}

				return res;
			}

			template <u64 _Size>
			inline constexpr i64 parse_zone_index(const char(&str)[_Size], u64& idx)
			{
				i64 zone_index_start = rfind_char(str, '%');
				if (zone_index_start > 0)
					return parse_address_component<10, '\0', std::numeric_limits<i64>::max()>(str, zone_index_start + 1);
				return -1;
			}

			template <i32 base, u32 max_value, i8 sep, u64 _Size>
			inline constexpr i32 parse_inet_component_base(const char(&str)[_Size], u64 idx)
			{
				return parse_address_component<base, sep, max_value>(str, idx);
			}

			template <u32 max_value, i8 sep, u64 _Size>
			inline constexpr i32 parse_inet_component_oct(const char(&str)[_Size], u64 idx)
			{
				return parse_inet_component_base<8, max_value, sep>(str, idx);
			}

			template <u32 max_value, i8 sep, u64 _Size>
			inline constexpr i32 parse_inet_component_dec(const char(&str)[_Size], u64 idx)
			{
				return parse_inet_component_base<10, max_value, sep>(str, idx);
			}

			template <u32 max_value, i8 sep, u64 _Size>
			inline constexpr i32 parse_inet_component_hex(const char(&str)[_Size], u64 idx)
			{
				return parse_inet_component_base<16, max_value, sep>(str, idx);
			}

			template <u32 max_value = 255, i8 sep, u64 _Size>
			inline constexpr i32 parse_inet_component(const char(&str)[_Size], u64 idx)
			{
				if ((_Size - idx) > 2ull && str[idx] == '0' && (toupper(str[idx + 1ull]) == 'X'))
					return parse_inet_component_hex<max_value, sep>(str, idx + 2ull);
				else if ((_Size - idx) > 2ull && str[idx] == '0' && isdigit(str[idx + 1ull]) && str[idx + 1ull] != '0')
					return parse_inet_component_oct<max_value, sep>(str, idx + 1ull);
				else
					return parse_inet_component_dec<max_value, sep>(str, idx);
			}

			template<u64 _Size>
			inline constexpr bool is_ipv4_embedded(const char(&str)[_Size], u64 idx) 
			{
				// Simple check for a dot, indicating an embedded IPv4 address
				for (u64 i = idx; i < _Size; i++) 
				{
					if (str[i] == '.')
						return true;
					if (str[i] == ':' || str[i] == '%')
						break;
				}
				return false;
			}

			template <class _Ty, u64 _Size>
			inline constexpr void rshift_array(std::array<_Ty, _Size>& a, u64 from, u64 shift)
			{
				if (from > _Size - 1ull)
					return;

				for (i64 pos = _Size - 1ull; pos >= static_cast<i64>(from + shift); pos--) 
				{
					if (pos - shift >= 0ull) 
					{
						a[pos] = a[pos - shift];
						a[pos - shift] = 0;
					}
					else
						a[pos] = static_cast<_Ty>(0);
				}
			}
		}

		class ipv4_address
		{
		public:
			using base_type = u8;
			static constexpr const u64 element_count{ 4ull };
			static constexpr const i8 separator{ '.' };
			static constexpr const u32 component_max = std::numeric_limits<base_type>::max();

			constexpr ipv4_address() = default;
			template<u64 _Size>
			constexpr ipv4_address(const char(&str)[_Size]) noexcept { parse_address(str); }
			template<u64 _Size>
			constexpr ipv4_address(const char(&str)[_Size], u64 pos) noexcept { parse_address(str, pos); }

			constexpr base_type& at(u64 pos) { return m_address[pos]; }
			constexpr const base_type& at(u64 pos) const  { return m_address[pos]; }
			constexpr base_type& operator[](u64 pos) noexcept { return m_address[pos]; }
			constexpr const base_type& operator[](u64 pos) const noexcept { return m_address[pos]; }

			constexpr std::array<base_type, element_count> to_bytes() const { return m_address; }
		protected:
			template<u64 _Size>
			constexpr void parse_address(const char(&str)[_Size], u64 start_pos = 0ull)
			{
				std::fill(m_address.begin(), m_address.end(), static_cast<base_type>(0));

				u64 pos{ start_pos };
				for (u64 idx = 0ull; idx < element_count; ++idx)
				{
					auto next_pos = parsing::find_char(str, pos, separator);
					if (next_pos == static_cast<u64>(-1)) next_pos = _Size;

					auto component = parsing::parse_inet_component<component_max, separator>(str, pos);
					if(component >= 0)
					{
						m_address[idx] = static_cast<base_type>(component);
						pos = next_pos + 1ull;
					}
				}
			}
		private:
			std::array<base_type, element_count> m_address;
		};

		class ipv6_address
		{
		public:
			using base_type = u16;
			static constexpr const u64 element_count{ 8ull };
			static constexpr const i8 separator{ ':' };
			static constexpr const u32 component_max = std::numeric_limits<base_type>::max();

			constexpr ipv6_address() = default;
			template<u64 _Size>
			constexpr ipv6_address(const char(&str)[_Size]) noexcept { parse_address(str); }
			template<u64 _Size>
			constexpr ipv6_address(const char(&str)[_Size], u64 pos) noexcept { parse_address(str, pos); }

			constexpr base_type& at(u64 pos) { return m_address[pos]; }
			constexpr const base_type& at(u64 pos) const { return m_address[pos]; }
			constexpr base_type& operator[](u64 pos) noexcept { return m_address[pos]; }
			constexpr const base_type& operator[](u64 pos) const noexcept { return m_address[pos]; }

			constexpr std::array<base_type, element_count> to_bytes() const { return m_address; }
		protected:
			template<u64 _Size>
			constexpr void parse_address(const char(&str)[_Size], u64 start_pos = 0ull)
			{
				std::fill(m_address.begin(), m_address.end(), static_cast<base_type>(0));

				i32 shortener_pos{ -1 };
				u64 pos{ start_pos };

				auto remaining_chars = [](u64 pos) constexpr { return _Size - 1ull - pos; };

				// The address must contain at least two chars, cannot start/end with a separator alone.
				if (_Size < 3ull || (str[0ull] == separator && str[1ull] != separator) || (str[_Size - 1ull] == separator && str[_Size - 2ull] != separator))
					return;

				for (u64 i = 0u; i < element_count; i++)
				{
					// We have reached the end of the string before parsing all the components.
					// That is possible only if we have previously encountered a shortener token.
					if (pos == _Size - 1ull)
					{
						if (shortener_pos == -1)
							return;
						else 
						{
							parsing::rshift_array(m_address, shortener_pos, element_count - i);
							break;
						}
					}

					// Check if we have an embedded IPv4 address.
					if ((i == 6ull || (i < 6ull && shortener_pos != -1)) && parsing::is_ipv4_embedded(str, pos))
					{
						// Parse embedded IPv4
						ipv4_address embedded_ipv4(str, pos);
						m_address[i++] = static_cast<base_type>((embedded_ipv4[0] << 8) | embedded_ipv4[1]);
						m_address[i++] = static_cast<base_type>((embedded_ipv4[2] << 8) | embedded_ipv4[3]);

						if (shortener_pos != -1)
							parsing::rshift_array(m_address, shortener_pos, element_count - i);

						pos = _Size - 1ull;
						break;
					}

					// A shortener token (::) is encountered.
					if (remaining_chars(pos) >= 2ull && str[pos] == separator && str[pos + 1ull] == separator)
					{
						// The address shortener syntax can only appear once.
						if (shortener_pos != -1)
							return;

						// It cannot be followed by another separator token.
						if (remaining_chars(pos) >= 3ull && str[pos + 2ull] == separator)
							return;

						// Save the component position where the token was found.
						shortener_pos = i;

						pos += 2ull;
					}
					else
					{
						i32 hexlet = parsing::parse_address_component<16, separator, component_max, 4>(str, pos);
						if (hexlet == -1)
							return;

						m_address[i] = hexlet;

						i64 next_sep = parsing::find_char(str, pos, separator);
						if (next_sep == -1)
							pos = _Size - 1ull;
						else if (remaining_chars(next_sep) >= 2ull && str[next_sep + 1ull] == separator)
							pos = next_sep;
						else
							pos = next_sep + 1;
					}
				}

				if (pos != _Size - 1ull)
					return;
			}
		private:
			std::array<base_type, element_count> m_address;
		};
	}
}