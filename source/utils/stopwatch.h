#pragma once

#include <standart_library.h>

namespace utl
{
	namespace utils
	{
        class stopwatch
        {
            using timepoint_t = stl::chrono::steady_clock::time_point;
        public:
            stopwatch() noexcept
            {
                sp = stl::chrono::high_resolution_clock::now();
            }

            template<class _Ty>
            _Ty stop()
            {
                auto now = stl::chrono::high_resolution_clock::now();
                auto secs = stl::chrono::duration<_Ty>(now - sp).count();
                sp = now;
                return secs;
            }
        private:
            timepoint_t sp;
        };
	}
}