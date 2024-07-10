#pragma once

#include <atomic>
#include <initializer_list>
#include <cstddef>
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
		buffer(std::size_t size, std::size_t align = alignof(std::max_align_t));
		~buffer();

		bool valid() const;

		uint16_t resize(std::size_t new_size, std::size_t align = alignof(std::max_align_t));

		std::size_t size() const;

		raw_memory_view get_raw_memory_view();
	private:
		void* memory_{ nullptr };
		std::size_t size_{ 0ull };
		std::size_t aligned_size_{ 0ull };
	};

	template<size_t _Size, size_t _Alignment = alignof(std::max_align_t)>
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

	template<size_t _Size, size_t _Alignment = alignof(std::max_align_t)>
	class ring_memory_pool
	{
	public:
		ring_memory_pool()
			: head_(0ull), tail_(0ull) {}

		void* allocate(std::size_t size)
		{
			size = aligned_size(size, _Alignment);
			std::size_t current_tail = tail_.load(std::memory_order_relaxed);
			std::size_t next_tail = (current_tail + size) % _Size;

			if (next_tail == head_.load(std::memory_order_acquire))
			{
				// Buffer is full
				return nullptr;
			}

			void* ptr = buffer_.get_raw_memory_view().data() + current_tail;
			tail_.store(next_tail, std::memory_order_release);
			return ptr;
		}

		void deallocate(void* ptr, std::size_t size)
		{
			// In a lock-free ring buffer, deallocation is typically a no-op,
			// as we only move the head and tail pointers. However, we can implement
			// a safety check if needed. The below code does not deallocate memory
			// but shows how you might move the head if necessary.

			size = aligned_size(size, _Alignment);
			std::size_t current_head = head_.load(std::memory_order_relaxed);

			// Optional: Add safety checks if needed
			// if (ptr != buffer_.get_raw_memory_view().data() + current_head) {
			//     throw std::runtime_error("Invalid deallocation");
			// }

			head_.store((current_head + size) % _Size, std::memory_order_release);
		}
		
	private:
		std::atomic<std::size_t> head_;
		std::atomic<std::size_t> tail_;
		small_buffer<_Size, _Alignment> buffer_;
	};

	template<typename _Ty, size_t _Count, size_t _Alignment = alignof(std::max_align_t)>
	class temp_allocator
	{
	public:
		using value_type = _Ty;

		temp_allocator() = default;

		_Ty* allocate(std::size_t n)
		{
			std::size_t size = aligned_size(n * sizeof(_Ty), _Alignment);
			void* ptr = buffer_.allocate(size);
			if (!ptr)
			{
				throw std::bad_alloc();
			}
			return static_cast<_Ty*>(ptr);
		}

		void deallocate(_Ty* p, std::size_t n)
		{
			std::size_t size = aligned_size(n * sizeof(_Ty), _Alignment);
			buffer_.deallocate(p, size);
		}
	private:
		ring_memory_pool<_Count * sizeof(_Ty), _Alignment> buffer_;
	};
}