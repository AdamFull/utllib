#include <memory/manager.hpp>

using namespace utl;

std::unique_ptr<memory_manager>memory_manager::_instance{ nullptr };

memory_manager::~memory_manager()
{
    fprintf(stderr, "Allocated %zu bytes(%zu times). Deallocated %zu bytes(%zu times).\n", m_iAllocated, m_iAllocations, m_iDeallocated, m_iDeallocations);
}