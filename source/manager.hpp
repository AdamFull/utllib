#pragma once
#include <memory>
#include <fstream>
#include <format>
#include <iomanip>
#include <sstream>
#include <string>
#include <algorithm>

namespace utl
{
    class memory_meter
    {
    protected:
        static std::unique_ptr<memory_meter> _instance;
    public:
        memory_meter()
        {
            _foutput.open(("allocation_info_" + get_time() + ".log").c_str());
        }

        ~memory_meter()
        {
            _foutput << std::format("Allocated {} bytes({} times). Deallocated {} bytes({} times).\n", _allocated, _allocations, _deallocated, _deallocations);
            _foutput.flush();
        }

        static std::unique_ptr<memory_meter> &getInstance()
        {
            if (!_instance)
                _instance = std::make_unique<memory_meter>(); //.reset(new _Ty());
            return _instance;
        }

        template <class _Ty>
        void allocate(const size_t _Count)
        {
            _allocated += _Count;
            _allocations++;
            _foutput << std::format("Allocated {} bytes for object \"{}\".\n", _Count * sizeof(_Ty), typeid(_Ty).name());
            _foutput.flush();
        }

        template <class _Ty>
        void deallocate(_Ty *const _Ptr, const size_t _Count)
        {
            _deallocated += _Count;
            _deallocations++;
            _foutput << std::format("Deallocated {} bytes for object \"{}\" ({}).\n", _Count * sizeof(_Ty), typeid(_Ty).name(), static_cast<void*>(_Ptr));
            _foutput.flush();
        }

        int64_t in_use() { return _allocated - _deallocated; }

        static std::string get_time()
        {
            auto time = std::time(nullptr);
            std::stringstream ss;
            ss << std::put_time(std::localtime(&time), "%F_%T"); // ISO 8601 without timezone information.
            auto s = ss.str();
            std::replace(s.begin(), s.end(), ':', '-');
            return s;
        }

    private:
        std::ofstream _foutput{};
        int64_t _allocated{0}, _deallocated{0}, _allocations{0}, _deallocations{0};
    };
}