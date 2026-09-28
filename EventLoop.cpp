#include "EventLoop.h"
#include "Channel.h"
#include "Logger.h"
#include <string.h>
#include <sys/syscall.h>
#include <vector>
EventLoop::EventLoop()
{
	socketpair(AF_LOCAL, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0, m_socketPair);
	m_wakeUpChannel = new Channel(m_socketPair[1]);
	m_wakeUpChannel->enableReading();
	m_wakeUpChannel->setReadCallBack(std::bind(&EventLoop::readWakeUpData, this));
	m_wakeUpChannel->setET();
	m_epoll.epollAdd(m_wakeUpChannel);
}
void EventLoop::add(Channel* channel)
{
	m_epoll.epollAdd(channel);
}
void EventLoop::remove(Channel* channel)
{
	m_epoll.epollRemove(channel);
}
void EventLoop::modify(Channel* channel)
{
	m_epoll.epollModify(channel);
}
void EventLoop::addTask(std::function<void()> task)
{
	std::unique_lock<std::mutex> mtLock(mtx);
	m_taskQueue.push(task);
	mtLock.unlock();
	wakeUp();
}
void EventLoop::readWakeUpData()
{
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	read(m_socketPair[1], buffer, sizeof(buffer));
}
void EventLoop::wakeUp()
{
	char buffer[] = "hello";
	send(m_socketPair[0], buffer, sizeof(buffer), 0);
}
void EventLoop::run()
{
	// LogMessage("EventLoop::run() thread is %d", syscall(SYS_gettid));
	while (true)
	{
		std::vector<Channel*> channels = m_epoll.wait();
		for (auto channel : channels)
			channel->handleEvent();
		while (true)
		{
			std::unique_lock<std::mutex> mtLock(mtx);
			if (m_taskQueue.empty())
				break;
			std::function<void()> task = m_taskQueue.front();
			m_taskQueue.pop();
			mtLock.unlock();
			task();
		}
	}
}
EventLoop::~EventLoop()
{
	delete m_wakeUpChannel;
	close(m_socketPair[0]);
	close(m_socketPair[1]);
}