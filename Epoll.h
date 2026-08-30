#ifndef __EPOLL__
#define __EPOLL__
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>
class Epoll
{
private:
	int m_epfd;
	static const int max = 1024;
	epoll_event m_events[max];

public:
	Epoll();
	void epollAdd(int fd);
	void epollRemove(int fd);
	std::vector<epoll_event> wait(int time = -1);
	~Epoll();
};
#endif