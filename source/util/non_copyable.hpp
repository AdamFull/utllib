#pragma once

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
}
