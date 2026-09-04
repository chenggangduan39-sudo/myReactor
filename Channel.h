#ifndef __CHANNEL__
#define __CHANNEL__
#include <functional>
#include <stdint.h>
class EventLoop;
class Channel
{
private:
	int m_fd;
	uint32_t m_event;
	uint32_t m_revent;
	std::function<void()> m_readCallBack;

public:
	Channel(int fd);
	void enableReading();
	void setRevent(uint32_t event);
	void setET();
	int getFd();
	void setReadCallBack(std::function<void()> readCallBack);
	void handleEvent();
};
#endif