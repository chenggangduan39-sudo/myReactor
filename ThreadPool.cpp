#include "ThreadPool.h"
#include "Logger.h"
#include <sys/syscall.h>
#include <unistd.h>
ThreadPool::ThreadPool(int num, std::string threadType) : m_threadType(threadType)
{
	for (int i = 0; i < num; i++)
	{
		workThreads.emplace_back([this]() {
			LogMessage("Create %s thread %d", m_threadType.c_str(), syscall(SYS_gettid));
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
				LogMessage("%s(%d) execute task completed", m_threadType.c_str(), syscall(SYS_gettid));
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