#pragma once

#include <standart_library.h>

namespace utl
{
	namespace utils
	{
        template <typename T> constexpr stl::string_view type_name();

        template <>
        constexpr stl::string_view type_name<void>() { return "void"; }

        namespace detail
        {
            using type_name_prober = void;

            template <typename T>
            constexpr inline stl::string_view wrapped_type_name() noexcept
            {
#ifdef __clang__
                return __PRETTY_FUNCTION__;
#elif defined(__GNUC__)
                return __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
                return __FUNCSIG__;
#else
#error "Unsupported compiler"
#endif
            }

            constexpr inline u64 wrapped_type_name_prefix_length() noexcept {
                return wrapped_type_name<type_name_prober>().find(type_name<type_name_prober>());
            }

            constexpr inline u64 wrapped_type_name_suffix_length() noexcept {
                return wrapped_type_name<type_name_prober>().length()
                    - wrapped_type_name_prefix_length()
                    - type_name<type_name_prober>().length();
            }

        } // namespace detail

        template <class _Ty>
        constexpr inline stl::string_view type_name()
        {
            constexpr auto wrapped_name = detail::wrapped_type_name<_Ty>();
            constexpr auto prefix_length = detail::wrapped_type_name_prefix_length();
            constexpr auto suffix_length = detail::wrapped_type_name_suffix_length();
            constexpr auto type_name_length = wrapped_name.length() - prefix_length - suffix_length;
            return wrapped_name.substr(prefix_length, type_name_length);
        }
	}
}