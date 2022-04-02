#pragma once
#include <memory/allocator.hpp>
#include <string>
#include <sstream>

namespace utl
{
    using string = std::basic_string<char, std::char_traits<char>, allocator<char>>;
    using wstring = std::basic_string<wchar_t, std::char_traits<wchar_t>, allocator<wchar_t>>;
    #ifdef __cpp_lib_char8_t
    using u8string = std::basic_string<char8_t, std::char_traits<char8_t>, allocator<char8_t>>;
    #endif // __cpp_lib_char8_t
    using u16string = std::basic_string<char16_t, std::char_traits<char16_t>, allocator<char16_t>>;
    using u32string = std::basic_string<char32_t, std::char_traits<char32_t>, allocator<char32_t>>;

    using stringbuf = std::basic_stringbuf<char, std::char_traits<char>, allocator<char>>;
    using istringstream = std::basic_istringstream<char, std::char_traits<char>, allocator<char>>;
    using ostringstream = std::basic_ostringstream<char, std::char_traits<char>, allocator<char>>;
    using stringstream = std::basic_stringstream<char, std::char_traits<char>, allocator<char>>;

    using wstringbuf = std::basic_stringbuf<wchar_t, std::char_traits<wchar_t>, allocator<wchar_t>>;
    using wistringstream = std::basic_istringstream<wchar_t, std::char_traits<wchar_t>, allocator<wchar_t>>;
    using wostringstream = std::basic_ostringstream<wchar_t, std::char_traits<wchar_t>, allocator<wchar_t>>;
    using wstringstream = std::basic_stringstream<wchar_t, std::char_traits<wchar_t>, allocator<wchar_t>>;

    class string_util
    {
        using avaliable_types_t = std::tuple<string, std::string_view, const char *, char *, int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t, double, float, bool>;
    public:
        template <class... _Types>
        static string conv(_Types &&...args)
        {
            stringstream stream;
            variadic_pass(stream, std::forward<_Types>(args)...);
            return stream.str();
        }

        template <class _Ty>
        static _Ty cast(const string &str)
        {
            static_assert(!std::is_same<_Ty, void>::value && std::is_fundamental<_Ty>::value, "Required fundamental type, but got wrong.");
            _Ty toret;
            stringstream ss;
            ss << str;
            ss >> toret;
            return toret;
        }

        template <class _Ty>
        static string name_of()
        {
            return typeid(_Ty).name();
        }

        template <class _Ty>
        static string name_of(const _Ty &t)
        {
            return typeid(t).name();
        }

    private:
        template <class _Ty>
        static void variadic_pass(std::ostream &stream, _Ty var)
        {
            static_assert(!std::is_same<_Ty, void>::value && !std::is_same<_Ty, avaliable_types_t>::value, "Type is not supported.");
            stream << var;
        }

        template <class _Ty, class... _Types>
        static void variadic_pass(std::ostream &stream, _Ty var, _Types... args)
        {
            variadic_pass(stream, var);
            variadic_pass(stream, std::forward<_Types>(args)...);
        }
    };
}