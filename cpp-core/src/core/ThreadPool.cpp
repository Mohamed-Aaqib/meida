#include "core/ThreadPool.h"

ThreadPool::ThreadPool(size_t num_threads) {
	
	for (int i = 0; i < num_threads; i++) {

		workers.emplace_back([this]() {
		
			while (true) {

				std::function<void()> task;
				{
					std::unique_lock<std::mutex> lock(queue_mutex);
					cv.wait(lock, [this] { return !tasks.empty() || stop; });
					// manually grabs the lock again
					if (stop && tasks.empty()) return;
					task = std::move(tasks.front());
					tasks.pop();
					{
						std::lock_guard<std::mutex> a_lock(active_mutex);
						active_tasks++;
					}
				}
				task();
				{
					std::lock_guard<std::mutex> a_lock(active_mutex);
					active_tasks--;
					if (active_tasks == 0) active_cv.notify_all();
				}
			}
			
		});

	}


}

ThreadPool::~ThreadPool() {

	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		stop = true;
	}
	cv.notify_all();
	for (auto& w : workers) w.join();

}


void ThreadPool::enqueue(std::function<void()> task) {

	{
		std::lock_guard<std::mutex> lock(queue_mutex);
		// emplace and emplace back are both useful shorthand
		tasks.emplace(std::move(task));
	}
	// notify one who is waiting
	cv.notify_one();

}

// block the main thread
void ThreadPool::waitCompletion() {

	std::unique_lock<std::mutex> lock(active_mutex);
	active_cv.wait(lock, [this]() {
		return tasks.empty() && active_tasks == 0;
	});

}


