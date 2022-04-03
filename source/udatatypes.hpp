#pragma once

#include <vector>
#include <deque>
#include <forward_list>
#include <list>

#include <set>
#include <map>

#include <unordered_set>
#include <unordered_map>

#include <stack>
#include <queue>

namespace utl
{
    template<class _Ty> class allocator;

    template <class _Ty, class _Alloc = allocator<_Ty>>
    using vector = std::vector<_Ty, _Alloc>;
    template <class _Ty, class _Alloc = allocator<_Ty>>
    using deque = std::deque<_Ty, _Alloc>;
    template <class _Ty, class _Alloc = allocator<_Ty>>
    using forwaed_list = std::forward_list<_Ty, _Alloc>;

    template <class _Kty, class _Pr = std::less<_Kty>, class _Alloc = allocator<_Kty>>
    using set = std::set<_Kty, _Pr, _Alloc>;
    template <class _Kty, class _Ty, class _Pr = std::less<_Kty>, class _Alloc = allocator<std::pair<const _Kty, _Ty>>>
    using map = std::map<_Kty, _Ty, _Pr, _Alloc>;
    template <class _Kty, class _Pr = std::less<_Kty>, class _Alloc = allocator<_Kty>>
    using multiset = std::multiset<_Kty, _Pr, _Alloc>;
    template <class _Kty, class _Ty, class _Pr = std::less<_Kty>, class _Alloc = allocator<std::pair<const _Kty, _Ty>>>
    using multimap = std::multimap<_Kty, _Ty, _Pr, _Alloc>;

    template <class _Kty, class _Hasher = std::hash<_Kty>, class _Keyeq = std::equal_to<_Kty>, class _Alloc = allocator<_Kty>>
    using unordered_set = std::unordered_set<_Kty, _Hasher, _Keyeq, _Alloc>;
    template <class _Kty, class _Ty, class _Hasher = std::hash<_Kty>, class _Keyeq = std::equal_to<_Kty>, class _Alloc = allocator<std::pair<const _Kty, _Ty>>>
    using unordered_map = std::unordered_map<_Kty, _Ty, _Hasher, _Keyeq, _Alloc>;
    template <class _Kty, class _Hasher = std::hash<_Kty>, class _Keyeq = std::equal_to<_Kty>, class _Alloc = allocator<_Kty>>
    using unordered_multiset = std::unordered_multiset<_Kty, _Hasher, _Keyeq, _Alloc>;
    template <class _Kty, class _Ty, class _Hasher = std::hash<_Kty>, class _Keyeq = std::equal_to<_Kty>, class _Alloc = allocator<std::pair<const _Kty, _Ty>>>
    using unordered_multimap = std::unordered_multimap<_Kty, _Ty, _Hasher, _Keyeq, _Alloc>;

    template <class _Ty, class _Container = deque<_Ty>>
    using stack = std::stack<_Ty, _Container>;
    template <class _Ty, class _Container = deque<_Ty>>
    using queue = std::queue<_Ty, _Container>;
    template <class _Ty, class _Container = vector<_Ty>, class _Pr = std::less<typename _Container::value_type>>
    using priority_queue = std::priority_queue<_Ty, _Container, _Pr>;

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
}