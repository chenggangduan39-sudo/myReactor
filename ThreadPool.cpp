#include "ThreadPool.h"
ThreadPool::ThreadPool(int num)
{
	for (int i = 0; i < num; i++)
	{
		workThreads.emplace_back([this]() {
			while (true)
			{
				std::unique_lock<std::mutex> mtLock(mtx);
				cond.wait(mtLock, [&]() {
					return !taskQueue.empty();
				});
				std::function<void()> task = taskQueue.front();
				taskQueue.pop();
				mtLock.unlock();
				task();
			}
		});
	}
}
void ThreadPool::addTask(std::function<void()> task)
{
	std::unique_lock<std::mutex> mtLock(mtx);
	taskQueue.push(task);
	mtLock.unlock();
	cond.notify_one();
}