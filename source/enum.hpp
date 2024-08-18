#pragma once

#if (201711 <= __cpp_impl_three_way_comparison) && __has_include(<compare> )
#define HAS_SPACESHIP_OPERATOR
#endif
#if defined(HAS_SPACESHIP_OPERATOR)
#include <compare>
#endif

#include <standart_library.h>

namespace utl
{
	template <typename FlagBitsType>
	struct FlagTraits
	{
	};

	template <typename BitType>
	class flags
	{
	public:
		using MaskType = typename std::underlying_type<BitType>::type;

		// constructors
		constexpr flags() noexcept : m_mask(0) {}

		constexpr flags(BitType bit) noexcept : m_mask(cast<MaskType>(bit)) {}

		constexpr flags(flags<BitType> const& rhs) noexcept = default;

		constexpr flags(MaskType flags) noexcept : m_mask(flags) {}

		// relational operators
#if defined(HAS_SPACESHIP_OPERATOR)
		auto operator<=>(flags<BitType> const&) const = default;
#else
		constexpr bool operator<(flags<BitType> const& rhs) const noexcept
		{
			return m_mask < rhs.m_mask;
		}

		constexpr bool operator<=(flags<BitType> const& rhs) const noexcept
		{
			return m_mask <= rhs.m_mask;
		}

		constexpr bool operator>(flags<BitType> const& rhs) const noexcept
		{
			return m_mask > rhs.m_mask;
		}

		constexpr bool operator>=(flags<BitType> const& rhs) const noexcept
		{
			return m_mask >= rhs.m_mask;
		}

		constexpr bool operator==(flags<BitType> const& rhs) const noexcept
		{
			return m_mask == rhs.m_mask;
		}

		constexpr bool operator!=(flags<BitType> const& rhs) const noexcept
		{
			return m_mask != rhs.m_mask;
		}
#endif

		// logical operator
		constexpr bool operator!() const noexcept
		{
			return !m_mask;
		}

		// bitwise operators
		constexpr flags<BitType> operator&(flags<BitType> const& rhs) const noexcept
		{
			return flags<BitType>(m_mask & rhs.m_mask);
		}

		constexpr flags<BitType> operator|(flags<BitType> const& rhs) const noexcept
		{
			return flags<BitType>(m_mask | rhs.m_mask);
		}

		constexpr flags<BitType> operator^(flags<BitType> const& rhs) const noexcept
		{
			return flags<BitType>(m_mask ^ rhs.m_mask);
		}

		constexpr flags<BitType> operator~() const noexcept
		{
			return flags<BitType>(m_mask ^ FlagTraits<BitType>::allflags);
		}

		// assignment operators
		constexpr flags<BitType>& operator=(flags<BitType> const& rhs) noexcept = default;

		constexpr flags<BitType>& operator|=(flags<BitType> const& rhs) noexcept
		{
			m_mask |= rhs.m_mask;
			return *this;
		}

		constexpr flags<BitType>& operator&=(flags<BitType> const& rhs) noexcept
		{
			m_mask &= rhs.m_mask;
			return *this;
		}

		constexpr flags<BitType>& operator^=(flags<BitType> const& rhs) noexcept
		{
			m_mask ^= rhs.m_mask;
			return *this;
		}

		// cast operators
		explicit constexpr operator bool() const noexcept
		{
			return !!m_mask;
		}

		explicit constexpr operator MaskType() const noexcept
		{
			return m_mask;
		}

	private:
		MaskType m_mask;
	};


	template <typename _Enum>
	inline constexpr std::array<std::pair<_Enum, std::string_view>, 0> enum_metadata;

	template <typename _Enum>
	constexpr bool has_enum_metadata = !enum_metadata<_Enum>.empty();
}

#if !defined(HAS_SPACESHIP_OPERATOR)
// relational operators only needed for pre C++20
template <typename BitType>
constexpr bool operator<(BitType bit, utl::flags<BitType> const& flags) noexcept
{
	return flags.operator>(bit);
}

template <typename BitType>
constexpr bool operator<=(BitType bit, utl::flags<BitType> const& flags) noexcept
{
	return flags.operator>=(bit);
}

template <typename BitType>
constexpr bool operator>(BitType bit, utl::flags<BitType> const& flags) noexcept
{
	return flags.operator<(bit);
}

template <typename BitType>
constexpr bool operator>=(BitType bit, utl::flags<BitType> const& flags) noexcept
{
	return flags.operator<=(bit);
}

template <typename BitType>
constexpr bool operator==(BitType bit, utl::flags<BitType> const& flags) noexcept
{
	return flags.operator==(bit);
}

template <typename BitType>
constexpr bool operator!=(BitType bit, utl::flags<BitType> const& flags) noexcept
{
	return flags.operator!=(bit);
}
#endif

// bitwise operators
template <typename BitType>
constexpr utl::flags<BitType> operator&(BitType bit, utl::flags<BitType> const& flags) noexcept
{
	return flags.operator&(bit);
}

template <typename BitType>
constexpr utl::flags<BitType> operator|(BitType bit, utl::flags<BitType> const& flags) noexcept
{
	return flags.operator|(bit);
}

template <typename BitType>
constexpr utl::flags<BitType> operator^(BitType bit, utl::flags<BitType> const& flags) noexcept
{
	return flags.operator^(bit);
}

#define UTL_REGISTER_ENUM_CLASS_AS_FLAG_TYPE(FlagsName, EnumType) \
using FlagsName = utl::flags<EnumType>; \
inline FlagsName operator|(EnumType a, EnumType b) { return FlagsName(utl::cast<FlagsName::MaskType>(a) | utl::cast<FlagsName::MaskType>(b)); } \
inline FlagsName operator&(EnumType a, EnumType b) { return FlagsName(utl::cast<FlagsName::MaskType>(a) & utl::cast<FlagsName::MaskType>(b)); } \
inline FlagsName operator^(EnumType a, EnumType b) { return FlagsName(utl::cast<FlagsName::MaskType>(a) ^ utl::cast<FlagsName::MaskType>(b)); } \
inline EnumType operator~(EnumType e) { using underlying = std::underlying_type_t<EnumType>; return static_cast<EnumType>(~static_cast<underlying>(e)); }

#define UTL_REGISTER_ENUM_CLASS(FlagType, ...) template<> inline constexpr std::array utl::enum_metadata<FlagType> = { __VA_ARGS__ };
#define UTL_ENUM_META(Enum, Str) std::pair{Enum, Str}