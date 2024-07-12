#pragma once

#include <atomic>
#include <initializer_list>
#include <cstddef>
#include <cassert>
#include <memory>
#include <new>

namespace utl
{
	namespace error_code
	{
		constexpr const size_t ok = 0ull;
		constexpr const size_t bad_alloc = (~0ull) - (size_t)(1 << 0);
	}

	inline constexpr void* ptr_offset(void* ptr, size_t offset)
	{
		return (void*)((char*)ptr + offset);
	}

	inline constexpr const void* ptr_offset(const void* ptr, size_t offset)
	{
		return (const void*)((const char*)ptr + offset);
	}

	inline constexpr std::ptrdiff_t ptr_distance(const void* start, const void* end)
	{
		return (const char*)end - (const char*)start;
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
		constexpr raw_memory_view(_Ty const& val) noexcept : data_{ &val }, size_{ sizeof(_Ty) } {}

		constexpr raw_memory_view(std::size_t size, void* data) : data_{ data }, size_{ size } {}

		template<typename _Ty, std::size_t _Size>
		constexpr raw_memory_view(_Ty (&arr)[_Size]) noexcept : data_{ &arr[0]}, size_{_Size * sizeof(_Ty)} {}

		template <typename _Ty>
		constexpr raw_memory_view(std::initializer_list<_Ty> const& list) noexcept : data_{ list.begin() }, size_{ list.size() * sizeof(_Ty) } {}

		template <typename _Ty, typename std::enable_if<std::is_const<_Ty>::value, int>::type = 0>
		constexpr raw_memory_view(std::initializer_list<typename std::remove_const<_Ty>::type> const& list) noexcept : data_{ list.begin() }, size_{ list.size() * sizeof(_Ty) } {}

		constexpr const void* begin() const noexcept { return data_; }
		constexpr const void* end() const noexcept { return ptr_offset(data_, size_); }

		constexpr bool check_bounds(void* ptr) const
		{
			return !(!ptr || ptr < begin() || ptr >= end());
		}

		// Runtime check functions
		template<typename _Ty>
		constexpr _Ty* as(std::size_t byte_offset)
		{
			assert((byte_offset + sizeof(_Ty) < size_) && "Pointer is out of bounds");
			return (_Ty*)get(byte_offset);
		}

		template<typename _Ty>
		constexpr const _Ty* as(std::size_t byte_offset) const
		{
			assert((byte_offset + sizeof(_Ty) < size_) && "Pointer is out of bounds");
			return (_Ty*)get(byte_offset);
		}

		constexpr void* get(std::size_t byte_offset)
		{
			assert((byte_offset < size_) && "Index out of bounds");
			return ptr_offset(data_, byte_offset);
		}

		constexpr const void* get(std::size_t byte_offset) const
		{
			assert((byte_offset < size_) && "Index out of bounds");
			return ptr_offset(data_, byte_offset);
		}

		constexpr bool empty() const noexcept { return (size_ == 0); }

		constexpr std::size_t size() const noexcept { return size_; }
		constexpr void const* data() const noexcept { return data_; }
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
		constexpr small_buffer() {}

		constexpr bool valid() const
		{
			return true;
		}

		constexpr std::size_t size()
		{
			return aligned_size(_Size, _Alignment);
		}

		constexpr raw_memory_view get_raw_memory_view()
		{
			return raw_memory_view(memory);
		}
	private:
		alignas(_Alignment) uint8_t memory[_Size];
	};

	namespace temporary_sizes
	{
		constexpr const size_t small_pool_size{ 256 };
		constexpr const size_t medium_pool_size{ 1024 };
		constexpr const size_t huge_pool_size{ 2048 };
	}

	template<size_t _InitialSize, size_t _Alignment = alignof(std::max_align_t)>
	class memory_pool
	{
	public:
		struct free_block;
		{
			std::size_t size{};
			free_block* next{ nullptr };
		};

		constexpr memory_pool() throw()
		{
			auto mem_view = buffer_.get_raw_memory_view();

			free_list_ = mem_view.as<free_block>(0ull);
			free_list_->size = mem_view.size();
			free_list_->next = nullptr;
		}

		[[nodiscard]] inline void* allocate(std::size_t count, std::size_t size)
		{
			std::size_t allocation_size = aligned_size(count * size, _Alignment);

			free_block* prev = nullptr;
			free_block* curr = free_list_;

			// Looking for free blocks
			while (current)
			{
				if (current->size >= allocation_size)
				{
					if (current->size > allocation_size + sizeof(free_block))
					{
						free_block* new_block = reinterpret_cast<free_block*>(ptr_offset(current, allocation_size));
						new_block->size = current->size - allocation_size - sizeof(free_block);
						new_block->next = current->next;
						current->next = new_block;
					}

					if (prev)
						prev->next = current->next;
					else
						free_list_ = current->next;

					current->size = allocation_size;
					return ptr_offset(current, sizeof(free_block));
				}
				prev = current;
				current = current->next;
			}

			grow(allocation_size);
			return allocate(count, size);
		}

		inline void* deallocate(void* ptr, std::size_t count, std::size_t size)
		{
			std::size_t allocation_size = aligned_size(count * size, _Alignment);

			free_block* block = reinterpret_cast<free_block*>(ptr_offset(ptr, -static_cast<ptrdiff_t>(sizeof(free_block))));
			block->size = allocation_size;

			free_block* current = free_list_;
			free_block* prev = nullptr;

			while (current && current < block) 
			{
				prev = current;
				current = current->next;
			}

			block->next = current;

			if (prev)
				prev->next = block;
			else
				free_list_ = block;

			merge_blocks();
		}
	protected:
		void grow(std::size_t required_size)
		{
			std::size_t current_size = buffer_.size();
			std::size_t new_size = current_size * 2ull;

			while (new_size < required_size + sizeof(free_block))
				new_size *= 2ull;

			buffer_.resize(new_size, _Alignment);

			free_block* new_block = buffer_.get_raw_memory_view().as<free_block>(current_size);
			new_block->size = new_size - current_size - sizeof(free_block);
			new_block->next = nullptr;

			free_block* current = free_list_;
			while (current && current->next)
				current = current->next;

			if (current)
				current->next = new_block;
			else
				free_list_ = new_block;

			merge_blocks();
		}

		void merge_blocks() 
		{
			free_block* current = free_list_;

			while (current && current->next) 
			{
				if (ptr_offset(current, current->size + sizeof(free_block)) == current->next) 
				{
					current->size += current->next->size + sizeof(free_block);
					current->next = current->next->next;
				}
				else
					current = current->next;
			}
		}

	private:
		buffer buffer_{ _InitialSize, _Alignment };
		free_block* free_list_{ nullptr };
	};

	template<size_t _Size, size_t _Alignment = alignof(std::max_align_t)>
	class ring_memory_pool
	{
	public:
		constexpr ring_memory_pool()
			: head_(0ull), tail_(0ull) {}

		[[nodiscard]] constexpr void* allocate(std::size_t size)
		{
			size = aligned_size(size, _Alignment);
			std::size_t current_tail = tail_.load(std::memory_order_relaxed);
			std::size_t next_tail = (current_tail + size) % _Size;

			if (next_tail == head_.load(std::memory_order_acquire))
			{
				// Buffer is full
				return nullptr;
			}

			void* ptr = buffer_.get_raw_memory_view().get(current_tail);
			tail_.store(next_tail, std::memory_order_release);
			return ptr;
		}

		constexpr void deallocate(void* ptr, std::size_t size)
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

	template<size_t _Alignment = alignof(std::max_align_t)>
	using small_temp_pool = ring_memory_pool<temporary_sizes::small_pool_size, _Alignment>;
	template<size_t _Alignment = alignof(std::max_align_t)>
	using medium_temp_pool = ring_memory_pool<temporary_sizes::medium_pool_size, _Alignment>;
	template<size_t _Alignment = alignof(std::max_align_t)>
	using huge_temp_pool = ring_memory_pool<temporary_sizes::huge_pool_size, _Alignment>;

	template<typename _Ty, size_t _Size, size_t _Alignment = alignof(std::max_align_t)>
	class temp_allocator
	{
	public:
		using value_type = _Ty;
		using size_type = size_t;
		using difference_type = ptrdiff_t;

		template<typename _Other>
		struct rebind 
		{
			using other = temp_allocator<_Other, _Size, _Alignment>;
		};

		constexpr temp_allocator(ring_memory_pool<_Size, _Alignment>& buffer) noexcept : buffer_(buffer) {}

		template<typename _Other>
		constexpr temp_allocator(const temp_allocator<_Other, _Size, _Alignment>& other) noexcept : buffer_(other.buffer_) {}

		[[nodiscard]] constexpr _Ty* allocate(std::size_t n)
		{
			std::size_t size = aligned_size(n * sizeof(_Ty), _Alignment);
			void* ptr = buffer_.allocate(size);
			if (!ptr)
				throw std::bad_alloc();

			return static_cast<_Ty*>(ptr);
		}

		constexpr void deallocate(_Ty* p, std::size_t n)
		{
			std::size_t size = aligned_size(n * sizeof(_Ty), _Alignment);
			buffer_.deallocate(p, size);
		}

		template<typename... _Args>
		constexpr void construct(_Ty* p, _Args&&... args)
		{
			if constexpr (!std::is_trivial_v<_Ty>)
				::new (static_cast<void*>(p)) _Ty(std::forward<_Args>(args)...);
		}

		constexpr void destroy(_Ty* p)
		{
			if constexpr (!std::is_trivial_v<_Ty>)
				p->~_Ty();
		}

		inline size_type max_size() const throw() { return _Size / sizeof(_Ty); }

		bool operator==(const temp_allocator& rhs) const noexcept
		{
			return &buffer_ == &rhs.buffer_;
		}

		bool operator!=(const temp_allocator& rhs) const noexcept
		{
			return !(*this == rhs);
		}
	private:
		template<typename _Other, size_t _OtherSize, size_t _OtherAlignment>
		friend class temp_allocator;

		ring_memory_pool<_Size, _Alignment>& buffer_;
	};

	template<typename _Ty, size_t _Alignment = alignof(std::max_align_t)>
	using small_temp_allocator = temp_allocator<_Ty, temporary_sizes::small_pool_size, _Alignment>;

	template<typename _Ty, size_t _Alignment = alignof(std::max_align_t)>
	using medium_temp_allocator = temp_allocator<_Ty, temporary_sizes::medium_pool_size, _Alignment>;

	template<typename _Ty, size_t _Alignment = alignof(std::max_align_t)>
	using huge_temp_allocator = temp_allocator<_Ty, temporary_sizes::huge_pool_size, _Alignment>;


	template<typename _Ty, size_t _Count, size_t _Alignment = alignof(std::max_align_t)>
	class pool_allocator
	{
	public:

	};
}

// Specialization of allocator_traits for temp_allocator
template<typename _Ty, size_t _Size, size_t _Alignment>
struct std::allocator_traits<utl::temp_allocator<_Ty, _Size, _Alignment>>
{
	using allocator_type = utl::temp_allocator<_Ty, _Size, _Alignment>;
	using value_type = typename allocator_type::value_type;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using void_pointer = void*;
	using const_void_pointer = const void*;
	using size_type = typename allocator_type::size_type;
	using difference_type = typename allocator_type::difference_type;

	template<typename _Other>
	using rebind_alloc = utl::temp_allocator<_Other, _Size, _Alignment>;

	static pointer allocate(allocator_type& a, size_type n)
	{
		return a.allocate(n);
	}

	static void deallocate(allocator_type& a, pointer p, size_type n)
	{
		a.deallocate(p, n);
	}

	template<typename _Ty, typename... Args>
	static void construct(allocator_type& a, _Ty* p, Args&&... args)
	{
		::new (static_cast<void*>(p)) _Ty(std::forward<Args>(args)...);
	}

	template<typename _Ty>
	static void destroy(allocator_type& a, _Ty* p)
	{
		p->~_Ty();
	}

	static size_type max_size(const allocator_type& a) noexcept
	{
		return std::numeric_limits<size_type>::max() / sizeof(value_type);
	}

	static allocator_type select_on_container_copy_construction(const allocator_type& rhs)
	{
		return rhs;
	}
};