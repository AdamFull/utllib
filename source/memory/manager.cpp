#include <memory/manager.hpp>

using namespace utl;

std::unique_ptr<memory_manager>memory_manager::_instance{ nullptr };

memory_manager::~memory_manager()
{
    fprintf(stderr, "Allocated %zu bytes. Deallocated %zu bytes.\n", m_iAllocated, m_iDeallocated);
}