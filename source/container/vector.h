#pragma once

#include <standart_library.h>

namespace utl
{
    // Based on https://github.com/sigerror/fast-vector/blob/master/vector.h
    //template <class T>
    //inline void construct_range(T *begin, T *end)
    //{
    //    while (begin != end)
    //    {
    //        new (begin) T;
    //        begin++;
    //    }
    //}
    //
    //template <class T>
    //inline void copy_range(T *begin, T *end, T *dest)
    //{
    //    while (begin != end)
    //    {
    //        new (dest) T(*begin);
    //        begin++;
    //        dest++;
    //    }
    //}
    //
    //template <class T>
    //inline void destruct_range(T *begin, T *end)
    //{
    //    while (begin != end)
    //    {
    //        begin->~T();
    //        begin++;
    //    }
    //}
    //
    //template <class _Ty>
    //class vector
    //{
    //public:
    //    using size_type = std::size_t;
    //
    //    vector() = default;
    //
    //    vector(const vector &other)
    //        : m_size(other.m_size), m_capacity(other.m_capacity)
    //    {
    //        m_data = reinterpret_cast<_Ty*>(std::malloc(sizeof(_Ty) * other.m_capacity));
    //
    //        if constexpr (std::is_trivial_v<_Ty>)
    //            std::memcpy(m_data, other.m_data, other.m_size);
    //        else
    //            copy_range(other.begin(), other.end(), m_data);
    //    }
    //
    //    vector(vector &&other) noexcept
    //        : m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity)
    //    {
    //        other.m_data = nullptr;
    //    }
    //
    //    vector &operator=(const vector &other)
    //    {
    //        m_size = other.m_size;
    //        m_capacity = other.m_capacity;
    //
    //        m_data = reinterpret_cast<_Ty*>(std::malloc(sizeof(_Ty) * other.m_capacity));
    //
    //        if constexpr (std::is_trivial_v<_Ty>)
    //            std::memcpy(m_data, other.m_data, other.m_size);
    //        else
    //            copy_range(other.begin(), other.end(), m_data);
    //
    //        return *this;
    //    }
    //
    //    vector &operator=(vector &&other) noexcept
    //    {
    //        m_data = other.m_data;
    //        m_size = other.m_size;
    //        m_capacity = other.m_capacity;
    //
    //        other.m_data = nullptr;
    //
    //        return *this;
    //    }
    //
    //    ~vector()
    //    {
    //        if constexpr (!std::is_trivial_v<_Ty>)
    //            destruct_range(begin(), end());
    //
    //        std::free(m_data);
    //    }
    //
    //    // Element access
    //
    //    _Ty &operator[](size_type pos)
    //    {
    //        assert(pos < m_size && "Position is out of range");
    //        return m_data[pos];
    //    }
    //
    //    const _Ty &operator[](size_type pos) const
    //    {
    //        assert(pos < m_size && "Position is out of range");
    //        return m_data[pos];
    //    }
    //
    //    _Ty &front()
    //    {
    //        assert(m_size > 0 && "Container is empty");
    //        return m_data[0];
    //    }
    //
    //    const _Ty &front() const
    //    {
    //        assert(m_size > 0 && "Container is empty");
    //        return m_data[0];
    //    }
    //
    //    _Ty &back()
    //    {
    //        assert(m_size > 0 && "Container is empty");
    //        return m_data[m_size - 1];
    //    }
    //
    //    const _Ty &back() const
    //    {
    //        assert(m_size > 0 && "Container is empty");
    //        return m_data[m_size - 1];
    //    }
    //
    //    _Ty* data() noexcept
    //    {
    //        return m_data;
    //    }
    //
    //    // TODO: cast to const pointer
    //    const _Ty* data() const noexcept
    //    {
    //        return m_data;
    //    }
    //
    //    // Iterators
    //    // TODO: add itterator impl
    //    _Ty* begin() noexcept
    //    {
    //        return m_data;
    //    }
    //
    //    const _Ty* begin() const noexcept
    //    {
    //        return m_data;
    //    }
    //
    //    _Ty* end() noexcept
    //    {
    //        return m_data + m_size;
    //    }
    //
    //    const _Ty* end() const noexcept
    //    {
    //        return m_data + m_size;
    //    }
    //
    //    // Capacity
    //
    //    bool empty() const noexcept
    //    {
    //        return m_size == 0;
    //    }
    //
    //    size_type size() const noexcept
    //    {
    //        return m_size;
    //    }
    //
    //    void reserve(size_type new_cap)
    //    {
    //        assert(new_cap > m_capacity && "Capacity is already equal to or greater than the passed value");
    //
    //        if constexpr (std::is_trivial_v<_Ty>)
    //        {
    //            m_data = reinterpret_cast<_Ty*>(std::realloc(m_data, sizeof(_Ty) * new_cap));
    //            assert(m_data != nullptr && "Reallocation failed");
    //        }
    //        else
    //        {
    //            _Ty *new_data_location = reinterpret_cast<_Ty*>(std::malloc(sizeof(_Ty) * new_cap));
    //            assert(new_data_location != nullptr && "Allocation failed");
    //
    //            copy_range(begin(), end(), new_data_location);
    //            destruct_range(begin(), end());
    //
    //            std::free(m_data);
    //
    //            m_data = new_data_location;
    //        }
    //
    //        m_capacity = new_cap;
    //    }
    //
    //    size_type capacity() const noexcept
    //    {
    //        return m_capacity;
    //    }
    //
    //    void shrink_to_fit()
    //    {
    //        if (m_size < m_capacity)
    //        {
    //            if constexpr (std::is_trivial_v<_Ty>)
    //            {
    //                m_data = reinterpret_cast<_Ty*>(std::realloc(m_data, sizeof(_Ty) * m_size));
    //                assert(m_data != nullptr && "Reallocation failed");
    //            }
    //            else
    //            {
    //                _Ty *new_data_location = reinterpret_cast<_Ty*>(std::malloc(sizeof(_Ty) * m_size));
    //                assert(new_data_location != nullptr && "Allocation failed");
    //
    //                copy_range(begin(), end(), new_data_location);
    //                destruct_range(begin(), end());
    //
    //                std::free(m_data);
    //
    //                m_data = new_data_location;
    //            }
    //        }
    //    }
    //
    //    // Modifiers
    //
    //    void clear() noexcept
    //    {
    //        if constexpr (!std::is_trivial_v<_Ty>)
    //            destruct_range(begin(), end());
    //
    //        m_size = 0;
    //    }
    //
    //    void push_back(const _Ty &value)
    //    {
    //        if (m_size == m_capacity)
    //            reserve(m_capacity * vector::grow_factor + 1);
    //
    //        if constexpr (std::is_trivial_v<_Ty>)
    //            m_data[m_size] = value;
    //        else
    //            new (m_data + m_size) _Ty(value);
    //
    //        m_size++;
    //    }
    //
    //    void push_back(_Ty &&value)
    //    {
    //        if (m_size == m_capacity)
    //            reserve(m_capacity * vector::grow_factor + 1);
    //
    //        if constexpr (std::is_trivial_v<_Ty>)
    //            m_data[m_size] = value;
    //        else
    //            new (m_data + m_size) _Ty(std::move(value));
    //
    //        m_size++;
    //    }
    //
    //    template <class... _Args>
    //    _Ty& emplace_back(_Args &&...args)
    //    {
    //        if constexpr (std::is_trivial_v<_Ty>)
    //            push_back(std::forward<_Args>(args)...);
    //        else
    //        {
    //            if (m_size == m_capacity)
    //                reserve(m_capacity * vector::grow_factor + 1);
    //
    //            new (m_data + m_size) _Ty(std::forward<_Args>(args)...);
    //
    //            m_size++;
    //        }
    //
    //        return m_data[m_size - 1];
    //    }
    //
    //    void pop_back()
    //    {
    //        assert(m_size > 0 && "Container is empty");
    //
    //        if constexpr (!std::is_trivial_v<_Ty>)
    //            m_data[m_size - 1].~_Ty();
    //
    //        m_size--;
    //    }
    //
    //    void resize(size_type count)
    //    {
    //        assert(count != m_size && "Size is already equal to the passed value");
    //
    //        if (count > m_capacity)
    //            reserve(count);
    //
    //        if constexpr (!std::is_trivial_v<_Ty>)
    //        {
    //            if (count > m_size)
    //                construct_range(m_data + m_size, m_data + count);
    //            else if (count < m_size)
    //                destruct_range(m_data + count, m_data + m_size);
    //        }
    //
    //        m_size = count;
    //    }
    //
    //    static constexpr size_type grow_factor = 2;
    //
    //private:
    //    _Ty* m_data{ nullptr };
    //    size_type m_size{ 0 };
    //    size_type m_capacity{ 0 };
    //};
}
