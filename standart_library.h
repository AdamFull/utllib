#pragma once

#include <cstddef>
#include <cstdint>
#include <format>
#include <mimalloc.h>

using i8 = int8_t;
using u8 = uint8_t;
using i16 = int16_t;
using u16 = uint16_t;
using i32 = int32_t;
using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;
using f32 = float;
using f64 = double;

#ifdef USE_EASTL_LIBRARY
#include <#ASTL/algorithm.h>
#include <#ASTL/allocator.h>
#include <#ASTL/allocator_malloc.h>
#include <#ASTL/any.h>
#include <#ASTL/array.h>
#include <#ASTL/atomic.h>
#include <#ASTL/bit.h>
#include <#ASTL/bitset.h>
//#include <#ASTL/bitvector.h>
#include <#ASTL/chrono.h>
#include <#ASTL/core_allocator.h>
#include <#ASTL/core_allocator_adapter.h>
#include <#ASTL/deque.h>
#include <#ASTL/finally.h>
#include <#ASTL/fixed_allocator.h>
//#include <#ASTL/fixed_function.h>
#include <#ASTL/functional.h>
//#include <#ASTL/hash_map.h>
//#include <#ASTL/hash_set.h>
#include <#ASTL/initializer_list.h>
//#include <#ASTL/intrusive_hash_map.h>
//#include <#ASTL/intrusive_hash_set.h>
//#include <#ASTL/intrusive_list.h>
//#include <#ASTL/intrusive_ptr.h>
//#include <#ASTL/linked_array.h>
//#include <#ASTL/linked_ptr.h>
#include <#ASTL/list.h>
#include <#ASTL/map.h>
#include <#ASTL/memory.h>
#include <#ASTL/numeric.h>
#include <#ASTL/numeric_limits.h>
#include <#ASTL/optional.h>
//#include <#ASTL/priority_queue.h>
#include <#ASTL/queue.h>
#include <#ASTL/random.h>
//#include <#ASTL/ratio.h>
//#include <#ASTL/safe_ptr.h>
//#include <#ASTL/scoped_array.h>
//#include <#ASTL/scoped_ptr.h>
//#include <#ASTL/segmented_vector.h>
#include <#ASTL/set.h>
//#include <#ASTL/shared_array.h>
#include <#ASTL/shared_ptr.h>
//#include <#ASTL/slist.h>
#include <#ASTL/sort.h>
#include <#ASTL/span.h>
#include <#ASTL/stack.h>
#include <#ASTL/string.h>
//#include <#ASTL/string_hash_map.h>
//#include <#ASTL/string_map.h>
#include <#ASTL/string_view.h>
#include <#ASTL/tuple.h>
#include <#ASTL/type_traits.h>
#include <#ASTL/unique_ptr.h>
#include <#ASTL/unordered_map.h>
#include <#ASTL/unordered_set.h>
#include <#ASTL/utility.h>
#include <#ASTL/variant.h>
#include <#ASTL/vector.h>
//#include <#ASTL/vector_map.h>
//#include <#ASTL/vector_multimap.h>
//#include <#ASTL/vector_multiset.h>
//#include <#ASTL/vector_set.h>
#include <#ASTL/weak_ptr.h>

namespace stl
{
	using namespace eastl;
}

#else

#include <algorithm>
#include <any>
#include <array>
#include <atomic>
#include <bit>
#include <bitset>
#include <chrono>
#include <deque>
#include <forward_list>
#include <functional>
#include <initializer_list>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <limits>
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <span>
#include <stack>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <valarray>
#include <variant>
#include <vector>
#include <iterator>

#include <exception>
#include <stdexcept>

namespace stl
{
	//template<class _Ty, class _Allocator = mi_stl_allocator<_Ty>>
	//using deque = std::deque<_Ty, _Allocator>;
	//
	//template<class _Ty, class _Allocator = mi_stl_allocator<_Ty>>
	//using list = std::list<_Ty, _Allocator>;
	//
	//template<class _KTy, class _Ty, class _Pr = std::less<_KTy>, class _Allocator = mi_stl_allocator<std::pair<_KTy, _Ty>>>
	//using map = std::map<_KTy, _Ty, _Pr, _Allocator>;
	//
	//template<class _Ty, class _Allocator = mi_stl_allocator<_Ty>>
	//using queue = std::queue<_Ty, _Allocator>;
	//
	//template<class _KTy, class _Pr = std::less<_KTy>, class _Allocator = mi_stl_allocator<_KTy>>
	//using set = std::set<_KTy, _Pr, _Allocator>;
	//
	//template<class _Ty, class _Allocator = mi_stl_allocator<_Ty>>
	//using make_mi_string = std::basic_string< _Ty, std::char_traits<_Ty>, _Allocator>;
	//
	//using string = make_mi_string<char>;
	//using wstring = make_mi_string<wchar_t>;
	//
	//template<class _KTy, class _Ty, class _Hasher = std::hash<_KTy>, class _Keyeq = std::equal_to<_KTy>, class _Allocator = mi_stl_allocator<std::pair<_KTy, _Ty>>>
	//using unordered_map = std::unordered_map<_KTy, _Hasher, _Keyeq, _Allocator>;
	//
	//template<class _KTy, class _Hasher = std::hash<_KTy>, class _Keyeq = std::equal_to<_KTy>, class _Allocator = mi_stl_allocator<_KTy>>
	//using unordered_set = std::unordered_set<_KTy, _Hasher, _Keyeq, _Allocator>;
	//
	//template<class _Ty, class _Allocator = mi_stl_allocator<_Ty>>
	//using vector = std::vector<_Ty, _Allocator>;

	using namespace std;
}

#endif
