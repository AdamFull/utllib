#pragma once

#include <random>
#include "ufstream.hpp"

namespace utl
{
    class non_copyable
    {
    protected:
        non_copyable() = default;
        virtual ~non_copyable() = default;

    public:
        non_copyable(const non_copyable &) = delete;
        non_copyable &operator=(const non_copyable &) = delete;
    };

    class non_movable
    {
    protected:
        non_movable() = default;
        virtual ~non_movable() = default;

    public:
        non_movable(non_movable &&) noexcept = delete;
        non_movable &operator=(non_movable &&) noexcept = delete;
    };

    class non_copy_movable : public non_copyable, public non_movable
    {
    protected:
        non_copy_movable() = default;
        virtual ~non_copy_movable() = default;
    };
}
