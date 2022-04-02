#pragma once
#include <util/non_copyable.hpp>
#include <util/non_movable.hpp>

namespace utl
{
    class non_copy_movable : public non_copyable, public non_movable
    {
    protected:
        non_copy_movable() = default;
        virtual ~non_copy_movable() = default;
    };
}
