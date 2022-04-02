#pragma once
#include <containers/string.hpp>

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
}
