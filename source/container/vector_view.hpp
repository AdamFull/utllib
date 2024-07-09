#pragma once

#include <standart_library.h>

namespace utl
{
	template<class _Ty>
	class vector_view
	{
	public:
		vector_view() : _data(nullptr), _size(0ull) {}
		vector_view(_Ty* data, u64 size) : _data(data), _size(size) {}

		vector_view(const vector_view& other) = default;
		vector_view& operator=(const vector_view& other) = default;
		vector_view(vector_view&& other) noexcept = default;
		vector_view& operator=(vector_view&& other) noexcept = default;

		~vector_view() = default;

		const _Ty& at(u64 index) const
		{
			if (index >= _size)
				throw std::out_of_range("Index out of range");

			return _data[index];
		}

		const _Ty& operator[](u64 index) const { return _data[index]; }

		const _Ty& front() const { return _data[0ull]; }
		const _Ty& back() const { return _data[_size - 1ull]; }
		const _Ty* data() const noexcept { return _data; }

		const _Ty* begin() const noexcept { return _data; }
		const _Ty* end() const noexcept { return _data + _size; }

		bool empty() const noexcept { return _size == 0ull; }
		u64 size() const noexcept { return _size; }
	private:
		_Ty* _data{ nullptr };
		u64 _size{ 0ull };
	};
}