#include "EventLoop.h"
#include "Channel.h"
#include <vector>
void EventLoop::add(Channel* channel)
{
	m_epoll.epollAdd(channel);
}
void EventLoop::remove(Channel* channel)
{
	m_epoll.epollRemove(channel);
}
void EventLoop::run()
{
	while (true)
	{
		std::vector<Channel*> channels = m_epoll.wait();
		for (auto channel : channels)
			channel->handleEvent();
	}
}