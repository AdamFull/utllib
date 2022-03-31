#pragma once
#include <util/non_copy_movable.hpp>

namespace utl
{
    namespace pattern
    {
        template<class T>
        class singleton : public non_copy_movable
        {
        //template<> std::unique_ptr<T> singleton<T>::_instance{ nullptr };
        protected:
            static std::unique_ptr<T> _instance;
        public:
            static std::unique_ptr<T>& getInstance()
            {
                if (!_instance)
                    _instance.reset(new T());
                return _instance;
            }
        };
    }
}
