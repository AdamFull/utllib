#pragma once
#include "uutility.hpp"
#include "umemory.hpp"

namespace utl
{
    template <class _Ty>
    class singleton : public non_copy_movable
    {
        // template<> unique_ptr<_Ty> singleton<_Ty>::_instance{ nullptr };
    protected:
        static unique_ptr<_Ty> _instance;

    public:
        static unique_ptr<_Ty> &getInstance()
        {
            if (!_instance)
                _instance.reset(new _Ty());
            return _instance;
        }
    };
}
