#pragma once

#include <standart_library.h>
#include <utils/hash.h>

namespace utl
{
	template<typename _Ty, typename _HashType>
	class id_string
	{
	public:
		using value_type = std::basic_string<_Ty, std::char_traits<_Ty>, std::allocator<_Ty>>;
		using hash_type = _HashType;

		id_string(const value_type& str)
			: _hash(compute_hash(str))
		{
			_store_string(_hash, str);
		}

		const _Ty* c_str() const
		{
			auto it = _string_pool().find(_hash);
			if (it != _string_pool().end())
				return it->second.c_str();

			return nullptr;
		}

		hash_type hash() const 
		{
			return _hash;
		}

	private:
		hash_type _hash;

		static hash_type compute_hash(const value_type& str)
		{
			if constexpr (sizeof(hash_type) == sizeof(u32))
			{
				return murmur_hash((char*)str.data(), str.size() * sizeof(_Ty), 0);
			}
			else if constexpr (sizeof(hash_type) == sizeof(u64))
			{
				return murmur_hash_64((char*)str.data(), str.size() * sizeof(_Ty), 0);
			}
		}

		static void _store_string(hash_type hash, const value_type& str)
		{
			_string_pool()[hash] = str;
		}

		static utl::hash_map<hash_type, std::string>& _string_pool()
		{
			static utl::hash_map<hash_type, std::basic_string<_Ty, > pool;
			return pool;
		}
	};

	using id_string32 = id_string<char, u32>;
	using id_string64 = id_string<char, u64>;

	using id_wstring32 = id_string<wchar_t, u32>;
	using id_wstring64 = id_string<wchar_t, u64>;
}