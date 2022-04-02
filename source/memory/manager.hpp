#pragma once
#include <memory>

namespace utl
{
    class memory_manager
    {
    protected:
        static std::unique_ptr<memory_manager> _instance;
    public:
        ~memory_manager();

        static std::unique_ptr<memory_manager> &getInstance()
        {
            if (!_instance)
                _instance.reset(new memory_manager());
            return _instance;
        }

        template <class _Ty>
        void allocate(const size_t _Count)
        {
            m_iAllocated += _Count;
            fprintf(stderr, "Allocated %zu bytes for object \"%s\".\n", _Count * sizeof(_Ty), typeid(_Ty).name());
        }

        template <class _Ty>
        void deallocate(_Ty *const _Ptr, const size_t _Count)
        {
            m_iDeallocated += _Count;
            fprintf(stderr, "Deallocated %zu bytes for object \"%s\" (%p).\n", _Count * sizeof(_Ty), typeid(_Ty).name(), _Ptr);
        }

    private:
        size_t m_iAllocated{0}, m_iDeallocated{0};
    };
}