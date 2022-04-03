#pragma once
#include "uutility.hpp"
#include "umemory.hpp"

namespace utl
{
    template <class _Ty>
    class singleton : public non_copy_movable
    {
        // template<> std::unique_ptr<_Ty> singleton<_Ty>::_instance{ nullptr };
    protected:
        static std::unique_ptr<_Ty> _instance;

    public:
        static std::unique_ptr<_Ty> &getInstance()
        {
            if (!_instance)
                _instance = make_unique<_Ty>(); //.reset(new _Ty());
            return _instance;
        }
    };
}
