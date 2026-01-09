#pragma once
#include <vector>
#include <thread>
#include <functional>
#include <mutex>
#include <queue>


class ThreadPool {

public:

	ThreadPool(size_t num_threads);

	void enqueue(std::function<void()> task);
	void waitCompletion();

	~ThreadPool();


private:

	std::vector<std::thread> workers;
	std::queue<std::function<void()>> tasks;
	
	std::mutex queue_mutex;
	std::condition_variable cv;

	std::mutex active_mutex;
	std::condition_variable active_cv;

	bool stop = false;
	size_t active_tasks = 0;

};