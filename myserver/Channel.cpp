#include "Channel.h"
#include "EventLoop.h"
#include "Logger.h"
#include <errno.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
Channel::Channel(int fd) : m_fd(fd), m_event(0), m_revent(0)
{
}
void Channel::enableReading()
{
	m_event = EPOLLIN;
}
void Channel::disableReading()
{
	m_event &= ~EPOLLIN;
}
void Channel::enableWriting()
{
	m_event |= EPOLLOUT;
}
void Channel::disableWriting()
{
	m_event &= ~EPOLLOUT;
}
void Channel::setRevent(uint32_t event)
{
	m_revent = event;
}
void Channel::setET()
{
	m_event |= EPOLLET;
}
int Channel::getFd()
{
	return m_fd;
}
uint32_t Channel::getEvent()
{
	return m_event;
}
void Channel::setReadCallBack(std::function<void()> readCallBack)
{
	m_readCallBack = readCallBack;
}
void Channel::setWriteCallBack(std::function<void()> writeCallBack)
{
	m_writeCallBack = writeCallBack;
}
void Channel::handleEvent()
{
	if (m_revent & EPOLLIN)
		m_readCallBack();
	if (m_revent & EPOLLOUT)
		m_writeCallBack();
}
