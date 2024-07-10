#pragma once

#include <initializer_list>
#include <cassert>

namespace utl
{
	namespace error_code
	{
		constexpr const size_t ok = 0ull;
		constexpr const size_t bad_alloc = (~0ull) - (size_t)(1 << 0);
	}

	inline void* ptr_offset(void* ptr, size_t offset)
	{
		return (void*)((char*)ptr + offset);
	}

	inline const void* ptr_offset(const void* ptr, size_t offset)
	{
		return (const void*)((const char*)ptr + offset);
	}

	constexpr std::size_t aligned_size(std::size_t size, std::size_t alignment) 
	{
		return (size + alignment - 1) & ~(alignment - 1);
	}

	class raw_memory_view
	{
	public:
		constexpr raw_memory_view() noexcept : data_{ nullptr }, size_{ 0ull } {}

		template <typename _Ty>
		raw_memory_view(_Ty const& val) noexcept : data_{ &val }, size_{ sizeof(_Ty) } {}

		raw_memory_view(std::size_t size, void* data) : data_{ data }, size_{ size } {}

		template<typename _Ty, std::size_t _Size>
		raw_memory_view(_Ty const(&arr)[_Size]) noexcept : data_{ arr }, size_{ _Size * sizeof(_Ty) } {}

		template <typename _Ty>
		raw_memory_view(std::initializer_list<_Ty> const& list) noexcept : data_{ list.begin() }, size_{ list.size() * sizeof(_Ty) } {}

		template <typename _Ty, typename std::enable_if<std::is_const<_Ty>::value, int>::type = 0>
		raw_memory_view(std::initializer_list<typename std::remove_const<_Ty>::type> const& list) noexcept : data_{ list.begin() }, size_{ list.size() * sizeof(_Ty) } {}

		const void* begin() const noexcept { return data_; }
		const void* end() const noexcept { return ptr_offset(data_, size_); }

		bool check_bounds(void* ptr) const
		{
			return !(!ptr || ptr < begin() || ptr >= end());
		}

		// Runtime check functions
		template<typename _Ty>
		_Ty* as(std::size_t byte_offset)
		{
			assert((byte_offset + sizeof(_Ty) < size_) && "Pointer is out of bounds");
			return (_Ty*)get(byte_offset);
		}

		template<typename _Ty>
		const _Ty* as(std::size_t byte_offset) const
		{
			assert((byte_offset + sizeof(_Ty) < size_) && "Pointer is out of bounds");
			return (_Ty*)get(byte_offset);
		}

		void* get(std::size_t byte_offset)
		{
			assert((byte_offset < size_) && "Index out of bounds");
			return ptr_offset(data_, byte_offset);
		}

		const void* get(std::size_t byte_offset) const
		{
			assert((byte_offset < size_) && "Index out of bounds");
			return ptr_offset(data_, byte_offset);
		}

		bool empty() const noexcept { return (size_ == 0); }

		std::size_t size() const noexcept { return size_; }
		void const* data() const noexcept { return data_; }
	private:
		void* data_{ nullptr };
		std::size_t size_{};
	};

	class buffer
	{
	public:
		buffer(std::size_t size, std::size_t align = 8ull);
		~buffer();

		bool valid() const;

		uint16_t resize(std::size_t new_size, std::size_t align = 8ull);

		std::size_t size() const;

		raw_memory_view get_raw_memory_view();
	private:
		void* memory_{ nullptr };
		std::size_t size_{ 0ull };
		std::size_t aligned_size_{ 0ull };
	};

	template<size_t _Size, size_t _Alignment = 8ull>
	class small_buffer
	{
	public:
		small_buffer() = default;

		bool valid() const
		{
			return true;
		}

		std::size_t size()
		{
			return aligned_size(_Size, _Alignment);
		}

		raw_memory_view get_raw_memory_view()
		{
			return raw_memory_view(_Size, memory);
		}
	private:
		alignas(_Alignment) uint8_t memory[_Size];
	};
}