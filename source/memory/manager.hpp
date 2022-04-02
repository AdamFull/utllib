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
            fprintf(stderr, "Allocated %zu bytes.\n", _Count * sizeof(_Ty));
        }

        template <class _Ty>
        void deallocate(_Ty *const _Ptr, const size_t _Count)
        {
            m_iDeallocated += _Count;
            fprintf(stderr, "Deallocated %zu bytes (%p).\n", _Count * sizeof(_Ty), _Ptr);
        }

    private:
        size_t m_iAllocated{0}, m_iDeallocated{0};
    };






    struct FreeStore 
    {
        FreeStore* next;
    };

    class IMemoryManager 
    {
    public:
        virtual void* allocate(size_t) = 0;
        virtual void free(void*) = 0;
    };

    class MemoryManager : public IMemoryManager
    {
    public: 
        MemoryManager( );
        virtual ~MemoryManager( );
        void* allocate(size_t) override;
        void free(void*) override;
    };
}