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
	std::function<void()> m_writeCallBack;

public:
	Channel(int fd);
	void enableReading();
	void disableReading();
	void enableWriting();
	void disableWriting();
	void setRevent(uint32_t event);
	void setET();
	int getFd();
	uint32_t getEvent();
	void setReadCallBack(std::function<void()> readCallBack);
	void setWriteCallBack(std::function<void()> writeCallBack);
	void handleEvent();
};
#endif