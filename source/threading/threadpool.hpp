#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <containers/cantainers.hpp>
#include <delegate/function.hpp>
#include <future>

namespace utl
{
    class worker
	{
	private:
		bool _destroying = false;
		std::thread _thread;
		queue<function<void()>> _work_queue;
		std::mutex _queue_mutex;
		std::condition_variable _condition;

		void loop()
		{
			while (true)
			{
				function<void()> work;
				{
					std::unique_lock<std::mutex> lock_begin(_queue_mutex);
					_condition.wait(lock_begin, [this] { return !_work_queue.empty() || _destroying; });
					if (_destroying) break;
					work = _work_queue.front();
				}

				work();

				{
					std::lock_guard<std::mutex> lock_end(_queue_mutex);
					_work_queue.pop();
					_condition.notify_one();
				}
			}
		}

	public:
		worker()
		{
			_thread = std::thread(&worker::loop, this);
		}

		~worker()
		{
			if (_thread.joinable())
			{
				wait();
				_queue_mutex.lock();
				_destroying = true;
				_condition.notify_one();
				_queue_mutex.unlock();
				_thread.join();
			}
		}

		template<class _Lambda>
		void push(_Lambda&& function)
		{
			std::lock_guard<std::mutex> lock(_queue_mutex);
			_work_queue.push(std::move(function));
			_condition.notify_one();
		}

		template<class _Lambda, class... _Types>
		void push(_Lambda&& function, _Types&& ...args)
		{
			push([function, ...args = std::forward<_Types>(args)] { function(std::forward<_Types>(args)...); });
		}

		// Wait until all work items have been finished
		void wait()
		{
			std::unique_lock<std::mutex> lock(_queue_mutex);
			_condition.wait(lock, [this]() { return _work_queue.empty(); });
		}
	};

    class threadpool
    {
    public:
        threadpool() { set_worker_count(std::thread::hardware_concurrency()); }
		threadpool(const size_t& count) { set_worker_count(count); }
		~threadpool() { wait(); }


        template<class... _Types>
        void push(_Types&& ...args)
        {
            _workers.at(_cworker++)->push(std::forward<_Types>(args)...);
            if(_cworker > _total - 1)
                _cworker = 0;
        }

		template<class _Ty, class... _Types, typename _Kty = std::invoke_result_t<std::decay_t<_Ty>, std::decay_t<_Types>...>, typename = std::enable_if_t<!std::is_void_v<_Ty>>>
		std::future<_Kty> submit(_Ty&& work, _Types&& ...args)
		{
			auto& worker = _workers.at(_cworker++);
			auto task_promise = make_shared<std::promise<_Kty>>();
			auto future = task_promise->get_future();

			worker->push([work = std::forward<_Ty>(work), args..., task_promise]()
			{
				try { task_promise->set_value(work(args...)); }
				catch(...)
				{
					try { task_promise->set_exception(std::current_exception()); }
                    catch (...) { }
				}
			});

			if(_cworker > _total - 1)
                _cworker = 0;

			return future;
		}

        void set_worker_count(const size_t& count)
        {
            _total = count;
            _workers.clear();
			for (auto i = 0; i < _total; i++)
				_workers.emplace_back(make_unique<worker>());
        }

        void wait()
        {
            for (auto &worker : _workers)
				worker->wait();
        }
    private:
        vector<unique_ptr<worker>> _workers;
        size_t _total{0}, _cworker{0};
    };
}