#pragma once
#include <fstream>
#include "udatatypes.hpp"

namespace utl
{
    template <class _Elem, class _Traits>
    class basic_ifstream : public std::basic_ifstream<_Elem, _Traits>
    {
    public:
        explicit basic_ifstream(const string& _Str, std::ios_base::openmode _Mode = std::ios_base::in, int _Prot = std::ios_base::_Default_open_prot)
        : basic_ifstream(_Str.c_str(), _Mode, _Prot) {} // _Prot is an extension

        explicit basic_ifstream(const wstring& _Str, std::ios_base::openmode _Mode = std::ios_base::in, int _Prot = std::ios_base::_Default_open_prot)
        : basic_ifstream(_Str.c_str(), _Mode, _Prot) {} // extension

        void open(const string& _Str, std::ios_base::openmode _Mode = std::ios_base::in, int _Prot = std::ios_base::_Default_open_prot) 
        {
            // _Prot is an extension
            open(_Str.c_str(), _Mode, _Prot);
        }

        void open(const wstring& _Str, std::ios_base::openmode _Mode = std::ios_base::in, int _Prot = std::ios_base::_Default_open_prot) 
        {
            // _Prot is an extension
            open(_Str.c_str(), _Mode, _Prot);
        }
    };

    template <class _Elem, class _Traits>
    class basic_ofstream : public std::basic_ofstream<_Elem, _Traits>
    {
    public:
        explicit basic_ofstream(const string& _Str, std::ios_base::openmode _Mode = std::ios_base::out, int _Prot = std::ios_base::_Default_open_prot)
        : basic_ofstream(_Str.c_str(), _Mode, _Prot) {} // _Prot is an extension

        explicit basic_ofstream(const wstring& _Str, std::ios_base::openmode _Mode = std::ios_base::out, int _Prot = std::ios_base::_Default_open_prot)
        : basic_ofstream(_Str.c_str(), _Mode, _Prot) {} // extension

        void open(const string& _Str, std::ios_base::openmode _Mode = std::ios_base::out, int _Prot = std::ios_base::_Default_open_prot) 
        {
            // _Prot is an extension
            open(_Str.c_str(), _Mode, _Prot);
        }

        void open(const wstring& _Str, std::ios_base::openmode _Mode = std::ios_base::out, int _Prot = std::ios_base::_Default_open_prot) 
        {
            // _Prot is an extension
            open(_Str.c_str(), _Mode, _Prot);
        }
    };

    using stringbuf = std::basic_stringbuf<char, std::char_traits<char>, allocator<char>>;
    using istringstream = std::basic_istringstream<char, std::char_traits<char>, allocator<char>>;
    using ostringstream = std::basic_ostringstream<char, std::char_traits<char>, allocator<char>>;
    using stringstream = std::basic_stringstream<char, std::char_traits<char>, allocator<char>>;
    using ifstream = basic_ifstream<char, std::char_traits<char>>;
    using ofstream = basic_ofstream<char, std::char_traits<char>>;

    using wstringbuf = std::basic_stringbuf<wchar_t, std::char_traits<wchar_t>, allocator<wchar_t>>;
    using wistringstream = std::basic_istringstream<wchar_t, std::char_traits<wchar_t>, allocator<wchar_t>>;
    using wostringstream = std::basic_ostringstream<wchar_t, std::char_traits<wchar_t>, allocator<wchar_t>>;
    using wstringstream = std::basic_stringstream<wchar_t, std::char_traits<wchar_t>, allocator<wchar_t>>;
    using wifstream = basic_ifstream<wchar_t, std::char_traits<wchar_t>>;
    using wofstream = basic_ofstream<wchar_t, std::char_traits<wchar_t>>;
}