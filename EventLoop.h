#ifndef __EVENTLOOP__
#define __EVENTLOOP__
#include "Epoll.h"
#include <functional>
#include <mutex>
#include <queue>
#include <sys/socket.h>
class Channel;
class EventLoop
{
private:
	Epoll m_epoll;
	int m_socketPair[2];
	Channel* m_wakeUpChannel;
	std::mutex mtx;
	std::queue<std::function<void()>> m_taskQueue;

public:
	EventLoop();
	void add(Channel* channel);
	void remove(Channel* channel);
	void modify(Channel* channel);
	void addTask(std::function<void()> task);
	void readWakeUpData();
	void wakeUp();
	void run();
	~EventLoop();
};
#endif