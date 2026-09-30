#include "Epoll.h"
#include "Channel.h"
Epoll::Epoll() : m_epfd(epoll_create(1))
{
}
void Epoll::epollAdd(Channel* channel)
{
	epoll_event ev;
	ev.data.ptr = channel;
	ev.events = channel->getEvent();
	epoll_ctl(m_epfd, EPOLL_CTL_ADD, channel->getFd(), &ev);
}
void Epoll::epollRemove(Channel* channel)
{
	epoll_event ev;
	ev.data.ptr = channel;
	ev.events = EPOLLIN | EPOLLET;
	epoll_ctl(m_epfd, EPOLL_CTL_DEL, channel->getFd(), &ev);
}
void Epoll::epollModify(Channel* channel)
{
	epoll_event ev;
	ev.data.ptr = channel;
	ev.events = channel->getEvent();
	epoll_ctl(m_epfd, EPOLL_CTL_MOD, channel->getFd(), &ev);
}
std::vector<Channel*> Epoll::wait(int time)
{
	int count = epoll_wait(m_epfd, m_events, max, -1);
	std::vector<Channel*> channels;
	for (int i = 0; i < count; i++)
	{
		Channel* channel = (Channel*)m_events[i].data.ptr;
		channel->setRevent(m_events[i].events);
		channels.push_back(channel);
	}
	return channels;
}
Epoll::~Epoll()
{
	close(m_epfd);
}