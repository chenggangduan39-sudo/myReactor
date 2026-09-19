#ifndef __EVENTLOOP__
#define __EVENTLOOP__
#include "Epoll.h"
class Channel;
class EventLoop
{
private:
	Epoll m_epoll;

public:
	void add(Channel* channel);
	void remove(Channel* channel);
	void modify(Channel* channel);
	void run();
};
#endif