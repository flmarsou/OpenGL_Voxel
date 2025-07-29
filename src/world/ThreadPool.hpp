#pragma once

# include "config.hpp"

# include <thread>
# include <mutex>
# include <functional>
# include <condition_variable>
# include <future>

# include <vector>
# include <queue>

class	ThreadPool
{
	public:
		ThreadPool(u32 threads);
		~ThreadPool();

		template<class F>
		void	Enqueue(F &&f);

		void	Wait();

	private:
		std::vector<std::thread>			_workers;
		std::queue<std::function<void()>>	_tasks;
		std::mutex							_queue_mutex;
		std::condition_variable				_condition;
		std::atomic<bool>					_stop = false;
		std::atomic<i32>					_active_tasks = 0;
};

inline	ThreadPool::ThreadPool(u32 threads)
{
	for (u32 i = 0; i < threads; i++)
	{
		this->_workers.emplace_back([this]
		{
			while (true)
			{
				std::function<void()>	task;
				{
					std::unique_lock<std::mutex>	lock(this->_queue_mutex);
					this->_condition.wait(lock, [this]{ return (this->_stop || !this->_tasks.empty()); });
					if (this->_stop && this->_tasks.empty())
						return ;
					task = std::move(this->_tasks.front());
					this->_tasks.pop();
					++this->_active_tasks;
				}
				task();
				--this->_active_tasks;
				this->_condition.notify_all();
			}
		});
	}
}

inline	ThreadPool::~ThreadPool()
{
	this->_stop = true;
	this->_condition.notify_all();
	for (std::thread &worker : this->_workers)
		worker.join();
}

template<class F>
void	ThreadPool::Enqueue(F &&f)
{
	{
		std::unique_lock<std::mutex>	lock(this->_queue_mutex);
		this->_tasks.emplace(std::forward<F>(f));
	}
	this->_condition.notify_one();
}

inline void	ThreadPool::Wait()
{
	std::unique_lock<std::mutex>	lock(this->_queue_mutex);
	this->_condition.wait(lock, [this]{ return (this->_tasks.empty() && this->_active_tasks == 0); });
}
