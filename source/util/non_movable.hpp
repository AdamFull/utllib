#pragma once

namespace utl
{
    namespace util
    {
        class non_movable
        {
        protected:
            non_movable() = default;
            virtual ~non_movable() = default;
        public:
            non_movable(non_movable&&) noexcept = delete;
            non_movable& operator=(non_movable&&) noexcept = delete;
        };
    }
}
