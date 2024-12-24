#pragma once

#include <standart_library.h>

namespace utl
{
	namespace utils
	{
        class stopwatch
        {
            using timepoint_t = std::chrono::steady_clock::time_point;
        public:
            stopwatch() noexcept
            {
                sp = std::chrono::high_resolution_clock::now();
            }

            template<class _Ty>
            _Ty stop()
            {
                auto now = std::chrono::high_resolution_clock::now();
                auto secs = std::chrono::duration<_Ty>(now - sp).count();
                sp = now;
                return secs;
            }
        private:
            timepoint_t sp;
        };

        template<class _Ty, class _KTy>
        class fps_counter
        {
        public:
            void update(_Ty dt)
            {
                if (accumulator > static_cast<_Ty>(1))
                {
                    meanFps = counter;
                    meanTime = accumulator / static_cast<_Ty>(counter);

                    accumulator = static_cast<_Ty>(0);
                    counter = static_cast<_KTy>(0);
                }
                else
                {
                    accumulator += dt;
                    counter += static_cast<_KTy>(1);
                }
            }

            const _Ty& delta_time() const
            {
                return meanTime;
            }

            const _KTy& fps() const
            {
                return meanFps;
            }
        private:
            _Ty accumulator{};
            _KTy counter{};
            _Ty meanTime{};
            _KTy meanFps{};
        };

        using fps_counter32 = fps_counter<f32, uint32_t>;
        using fps_counter64 = fps_counter<f64, uint64_t>;
	}
}