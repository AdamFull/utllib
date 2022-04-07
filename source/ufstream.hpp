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
        : std::basic_ifstream(_Str.c_str(), _Mode, _Prot) {} // _Prot is an extension

        explicit basic_ifstream(const wstring& _Str, std::ios_base::openmode _Mode = std::ios_base::in, int _Prot = std::ios_base::_Default_open_prot)
        : std::basic_ifstream(_Str.c_str(), _Mode, _Prot) {} // extension

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
        : std::basic_ofstream(_Str.c_str(), _Mode, _Prot) {} // _Prot is an extension

        explicit basic_ofstream(const wstring& _Str, std::ios_base::openmode _Mode = std::ios_base::out, int _Prot = std::ios_base::_Default_open_prot)
        : std::basic_ofstream(_Str.c_str(), _Mode, _Prot) {} // extension

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

    template<class _Elem, class _Traits>
    class basic_fstream : public std::basic_fstream<_Elem, _Traits>
    {
    public:
        explicit basic_fstream(const string& _Str, std::ios_base::openmode _Mode = std::ios_base::in | std::ios_base::out, int _Prot = std::ios_base::_Default_open_prot)
        : std::basic_fstream(_Str.c_str(), _Mode, _Prot) {} // _Prot is an extension

        explicit basic_fstream(const wstring& _Str, std::ios_base::openmode _Mode = std::ios_base::in | std::ios_base::out, int _Prot = std::ios_base::_Default_open_prot)
        : std::basic_fstream(_Str.c_str(), _Mode, _Prot) {} // extension

        void open(const string& _Str, std::ios_base::openmode _Mode = std::ios_base::in | std::ios_base::out, int _Prot = std::ios_base::_Default_open_prot) 
        {
            // extension
            open(_Str.c_str(), _Mode, _Prot);
        }

        void open(const wstring& _Str, std::ios_base::openmode _Mode = std::ios_base::in | std::ios_base::out, int _Prot = std::ios_base::_Default_open_prot) 
        {
            // extension
            open(_Str.c_str(), _Mode, _Prot);
        }
    };

    using ios = std::basic_ios<char, std::char_traits<char>>;
    using streambuf = std::basic_streambuf<char, std::char_traits<char>>;
    using istream = std::basic_istream<char, std::char_traits<char>>;
    using ostream = std::basic_ostream<char, std::char_traits<char>>;
    using iostream = std::basic_iostream<char, std::char_traits<char>>;
    using stringbuf = std::basic_stringbuf<char, std::char_traits<char>, allocator<char>>;
    using istringstream = std::basic_istringstream<char, std::char_traits<char>, allocator<char>>;
    using ostringstream = std::basic_ostringstream<char, std::char_traits<char>, allocator<char>>;
    using stringstream = std::basic_stringstream<char, std::char_traits<char>, allocator<char>>;
    using ifstream = basic_ifstream<char, std::char_traits<char>>;
    using ofstream = basic_ofstream<char, std::char_traits<char>>;
    using fstream = basic_fstream<char, std::char_traits<char>>;

    using wios = std::basic_ios<wchar_t, std::char_traits<wchar_t>>;
    using wstreambuf = std::basic_streambuf<wchar_t, std::char_traits<wchar_t>>;
    using wistream = std::basic_istream<wchar_t, std::char_traits<wchar_t>>;
    using wostream = std::basic_ostream<wchar_t, std::char_traits<wchar_t>>;
    using wiostream = std::basic_iostream<wchar_t, std::char_traits<wchar_t>>;
    using wstringbuf = std::basic_stringbuf<wchar_t, std::char_traits<wchar_t>, allocator<wchar_t>>;
    using wistringstream = std::basic_istringstream<wchar_t, std::char_traits<wchar_t>, allocator<wchar_t>>;
    using wostringstream = std::basic_ostringstream<wchar_t, std::char_traits<wchar_t>, allocator<wchar_t>>;
    using wstringstream = std::basic_stringstream<wchar_t, std::char_traits<wchar_t>, allocator<wchar_t>>;
    using wifstream = basic_ifstream<wchar_t, std::char_traits<wchar_t>>;
    using wofstream = basic_ofstream<wchar_t, std::char_traits<wchar_t>>;
    using wfstream = basic_fstream<wchar_t, std::char_traits<wchar_t>>;
}