#include "EventLoop.h"
#include "Channel.h"
#include "Logger.h"
#include <sys/syscall.h>
#include <vector>
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
void EventLoop::run()
{
	// LogMessage("EventLoop::run() thread is %d", syscall(SYS_gettid));
	while (true)
	{
		std::vector<Channel*> channels = m_epoll.wait();
		for (auto channel : channels)
			channel->handleEvent();
	}
}