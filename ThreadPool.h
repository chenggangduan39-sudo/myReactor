#ifndef __THREADPOOL__
#define __THREADPOOL__
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
class ThreadPool
{
private:
	std::mutex mtx;
	std::condition_variable cond;
	std::queue<std::function<void()>> taskQueue;
	std::vector<std::thread> workThreads;

public:
	ThreadPool(int num);
	void addTask(std::function<void()> task);
};
#endif