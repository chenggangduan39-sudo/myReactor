#include "Epoll.h"
Epoll::Epoll() : m_epfd(epoll_create(1))
{
}
void Epoll::epollAdd(int fd)
{
	epoll_event ev;
	ev.data.fd = fd;
	ev.events = EPOLLIN | EPOLLET;
	epoll_ctl(m_epfd, EPOLL_CTL_ADD, fd, &ev);
}
void Epoll::epollRemove(int fd)
{
	epoll_event ev;
	ev.data.fd = fd;
	ev.events = EPOLLIN | EPOLLET;
	epoll_ctl(m_epfd, EPOLL_CTL_DEL, fd, &ev);
}
std::vector<epoll_event> Epoll::wait(int time)
{
	int count = epoll_wait(m_epfd, m_events, max, -1);
	std::vector<epoll_event> events;
	for (int i = 0; i < count; i++)
	{
		events.push_back(m_events[i]);
	}
	return events;
}
Epoll::~Epoll()
{
	close(m_epfd);
}