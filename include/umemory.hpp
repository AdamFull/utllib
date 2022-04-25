#pragma once
#include "manager.hpp"

namespace utl
{
    template <class _Ty>
    class allocator : public std::allocator<_Ty>
    {
    public:
        using _From_primary = allocator;

        using value_type = _Ty;

#if _HAS_DEPRECATED_ALLOCATOR_MEMBERS
        _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS typedef _Ty *pointer;
        _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS typedef const _Ty *const_pointer;

        _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS typedef _Ty &reference;
        _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS typedef const _Ty &const_reference;
#endif // _HAS_DEPRECATED_ALLOCATOR_MEMBERS

        using size_type = size_t;
        using difference_type = ptrdiff_t;

        constexpr allocator() noexcept {}

        constexpr allocator(const allocator&) noexcept = default;
        template <class _Other>
        constexpr allocator(const allocator<_Other>&) noexcept {}
        constexpr ~allocator() = default;
        constexpr allocator& operator=(const allocator&) = default;

        _NODISCARD constexpr __declspec(allocator) _Ty *allocate(_CRT_GUARDOVERFLOW const size_t _Count)
        {
            //memory_meter::getInstance()->allocate<_Ty>(_Count * sizeof(_Ty));
            return std::allocator<_Ty>::allocate(_Count);
        }

        constexpr void deallocate(_Ty *const _Ptr, const size_t _Count)
        {
            //memory_meter::getInstance()->deallocate<_Ty>(_Ptr, _Count * sizeof(_Ty));
            return std::allocator<_Ty>::deallocate(_Ptr, _Count);
        }

#if _HAS_DEPRECATED_ALLOCATOR_MEMBERS
        _CXX17_DEPRECATE_OLD_ALLOCATOR_MEMBERS _NODISCARD __declspec(allocator) _Ty *allocate(_CRT_GUARDOVERFLOW const size_t _Count, const void *)
        {
            return allocate(_Count);
        }
#endif
    };

    template <class _Ty, class _Uy>
    inline bool operator == (const allocator<_Ty>&, const allocator<_Uy>&) { return true; }

    template <typename _Ty, typename _Uy>
    inline bool operator != (const allocator<_Ty>& a, const allocator<_Uy>& b) { return !(a == b); }

    template<typename _Alloc>
    struct alloc_deleter
    {
        constexpr alloc_deleter() noexcept = default;

        template <class _Uty, std::enable_if_t<std::is_convertible_v<_Uty (*)[], _Alloc (*)[]>, int> = 0>
        alloc_deleter(const alloc_deleter<_Uty[]>&) noexcept {}

        alloc_deleter(const _Alloc& allocator) : allocator(allocator) { }
        alloc_deleter &operator=(const _Alloc& allocator) { this->allocator = allocator; return *this; }
        alloc_deleter(_Alloc&& allocator) : allocator(std::move(allocator)) {}
        alloc_deleter &operator=(_Alloc&& allocator) noexcept { this->allocator = std::move(allocator); return *this; }

        alloc_deleter(const alloc_deleter& deleter) = default;
        alloc_deleter &operator=(const alloc_deleter& deleter) = default;
        alloc_deleter(alloc_deleter&& deleter) noexcept = default;
        alloc_deleter &operator=(alloc_deleter&& deleter) noexcept = default;

        typedef typename std::allocator_traits<_Alloc>::pointer pointer;

        void operator()(pointer ptr) const
        {
            _Alloc nallocator(allocator);
            std::allocator_traits<_Alloc>::destroy(nallocator, std::addressof(*ptr));
            std::allocator_traits<_Alloc>::deallocate(nallocator, ptr, 1);
        }

        private:
        _Alloc allocator;
    };

    template <class _Ty, class... _Types>
    _NODISCARD std::shared_ptr<_Ty> make_shared(_Types&&... args)
    {
        return std::allocate_shared<_Ty>(allocator<_Ty>(), std::forward<_Types>(args)...);
    }

    template<class _Ty, class _Alloc, class... _Types>
    _NODISCARD auto allocate_unique(const _Alloc& alloc, _Types&&... args) -> decltype(auto)
    {
        static_assert(std::is_same<typename std::allocator_traits<_Alloc>::value_type, std::remove_cv_t<_Ty>>{}(), "Allocator has the wrong value_type");

        _Alloc allocator(alloc);
        auto pointer = std::allocator_traits<_Alloc>::allocate(allocator, 1);
        try {
            std::allocator_traits<_Alloc>::construct(allocator, std::addressof(*pointer), std::forward<_Types>(args)...);
            return std::unique_ptr<_Ty, alloc_deleter<_Alloc>>(pointer, alloc_deleter<_Alloc>(allocator));
        }
        catch (...)
        {
            std::allocator_traits<_Alloc>::deallocate(allocator, pointer, 1);
            throw;
        }
    }

    template <class _Ty, class... _Types>
    _NODISCARD auto make_unique(_Types&&... args) -> decltype(auto)
    {
        return allocate_unique<_Ty>(allocator<_Ty>(), std::forward<_Types>(args)...);
    }

    template<class _Ty, class _Alloc = allocator<_Ty>, class _Dx = alloc_deleter<_Alloc>>
    using unique_ptr = std::unique_ptr<_Ty, _Dx>;
}