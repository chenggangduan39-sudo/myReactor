#include "Connection.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Logger.h"
#include "Socket.h"
#include <string.h>
Connection::Connection(int fd, InetAddress* clientAddr, EventLoop* evloop) : m_clientAddr(clientAddr), m_evloop(evloop)
{
	m_clientSock = new Socket(fd);
	m_clientSock->setNonBlock();
	m_clientChannel = new Channel(fd);
	m_clientChannel->enableReading();
	m_clientChannel->setET();
	m_clientChannel->setReadCallBack(std::bind(&Connection::recieveMessage, this));
	m_clientChannel->setWriteCallBack(std::bind(&Connection::sendMessage, this));
	LogMessage("Accept client:%s %d", m_clientAddr->getIP(), m_clientAddr->getPort());
	evloop->add(m_clientChannel);
}
void Connection::recieveMessage()
{
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	int num = 0;
	while (num = read(m_clientSock->getFd(), buffer, sizeof(buffer)))
	{
		if (num <= 0)
			break;
		m_inputBuffer.append(buffer, num);
		// LogMessage(buffer);
		// send(m_clientSock->getFd(), buffer, num, 0);
	}
	if (num == 0)
	{
		LogMessage("Disconnected");
		m_evloop->remove(m_clientChannel);
		notifyToDisconnect(m_clientSock->getFd());
	}
	else if ((num == -1 && errno == EAGAIN) || (num == -1 && errno == EWOULDBLOCK))
		m_handleCallBack(this);
	else if (num == -1 && errno != EAGAIN && errno != EWOULDBLOCK)
	{
		LogMessage("Something is wrong");
		m_evloop->remove(m_clientChannel);
		notifyToDisconnect(m_clientSock->getFd());
	}
}
void Connection::setCallBack(std::function<void(int)> callBack)
{
	m_callBack = callBack;
}
void Connection::setHandleCallBack(std::function<void(Connection*)> handleCallBack)
{
	m_handleCallBack = handleCallBack;
}
Buffer* Connection::getInputBuffer()
{
	return &m_inputBuffer;
}
Buffer* Connection::getOutputBuffer()
{
	return &m_outputBuffer;
}
void Connection::sendMessage()
{
	while (!m_outputBuffer.isEmpty())
	{
		int res = send(m_clientSock->getFd(), m_outputBuffer.data(), m_outputBuffer.size(), 0);
		if ((res == -1 && errno == EAGAIN) || (res == -1 && errno == EWOULDBLOCK))
		{
			m_clientChannel->enableWriting();
			m_evloop->modify(m_clientChannel);
			return;
		}
		else if (res > 0)
			m_outputBuffer.erase(0, res);
	}
	m_clientChannel->disableWriting();
	m_evloop->modify(m_clientChannel);
}
void Connection::notifyToDisconnect(int cfd)
{
	m_callBack(cfd);
}
Connection::~Connection()
{
	delete m_clientAddr;
	delete m_clientChannel;
	delete m_clientSock;
}