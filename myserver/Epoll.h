#ifndef __EPOLL__
#define __EPOLL__
#include <sys/epoll.h>
#include <unistd.h>
#include <vector>
class Channel;
class Epoll
{
private:
	int m_epfd;
	static const int max = 1024;
	epoll_event m_events[max];

public:
	Epoll();
	void epollAdd(Channel* channel);
	void epollRemove(Channel* chanenl);
	void epollModify(Channel* channel);
	std::vector<Channel*> wait(int time = -1);
	~Epoll();
};
#endif