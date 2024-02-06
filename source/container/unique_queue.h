#pragma once

#include <standart_library.h>

namespace utl
{
	namespace container
	{
        template<class _Ty>
        class unique_queue
        {
        public:
            void push(const _Ty& value)
            {
                if (_unique_values.insert(value).second)
                    _q.push(value);
            }

            void pop() {
                if (!_q.empty())
                {
                    _unique_values.erase(_q.front());
                    _q.pop();
                }
            }

            const _Ty& front() const
            {
                return _q.front();
            }

            const _Ty& back() const
            {
                return _q.back();
            }

            bool empty() const
            {
                return _q.empty();
            }

            u64 size() const
            {
                return _q.size();
            }
        private:
            stl::queue<_Ty> _q;
            stl::unordered_set<_Ty> _unique_values;
        };
	}
}