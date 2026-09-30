#include "Connection.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Logger.h"
#include "Socket.h"
#include "InetAddress.h"
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>
Connection::Connection(int fd, InetAddress* clientAddr, EventLoop* evloop)
    : m_clientAddr(clientAddr), m_evloop(evloop), isValid(true)
{
	m_clientSock = new Socket(fd);
	m_clientSock->setNonBlock();
	m_clientChannel = new Channel(fd);
	m_clientChannel->enableReading();
	m_clientChannel->setET();
	m_clientChannel->setReadCallBack(std::bind(&Connection::recieveMessage, this));
	m_clientChannel->setWriteCallBack(std::bind(&Connection::sendData, this));
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
		isValid = false;
		LogMessage("Disconnected");
		m_evloop->remove(m_clientChannel);
		notifyToDisconnect();
	}
	else if ((num == -1 && errno == EAGAIN) || (num == -1 && errno == EWOULDBLOCK))
		m_handleCallBack(shared_from_this());
	else if (num == -1 && errno != EAGAIN && errno != EWOULDBLOCK)
	{
		isValid = false;
		LogMessage("Something is wrong");
		m_evloop->remove(m_clientChannel);
		notifyToDisconnect();
	}
}
void Connection::setCallBack(std::function<void(int)> callBack)
{
	m_callBack = callBack;
}
void Connection::setHandleCallBack(std::function<void(sharedPtrConn)> handleCallBack)
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
void Connection::sendMessage(std::string message)
{
	LogMessage("Connection::sendMessage() thread is %d", syscall(SYS_gettid));
	auto self = shared_from_this();
	m_evloop->addTask(std::bind(&Connection::sendTask, self, message));
}
void Connection::sendTask(std::string message)
{
	if (isValid == false)
	{
		LogMessage("Sending task has been stoped:invalid connection(%d)", syscall(SYS_gettid));
		return;
	}
	m_outputBuffer.append(message.data(), message.size());
	m_clientChannel->enableWriting();
	m_evloop->modify(m_clientChannel);
}
void Connection::sendData()
{
	LogMessage("Connection::sendData() thread is %d", syscall(SYS_gettid));
	if (isValid == false)
	{
		LogMessage("Sending data has been stoped:invalid connection(%d)", syscall(SYS_gettid));
		return;
	}
	while (!m_outputBuffer.isEmpty())
	{
		int res = send(m_clientSock->getFd(), m_outputBuffer.data(), m_outputBuffer.size(), 0);
		if ((res == -1 && errno == EAGAIN) || (res == -1 && errno == EWOULDBLOCK))
			return;
		else if (res > 0)
			m_outputBuffer.erase(0, res);
	}
	m_clientChannel->disableWriting();
	m_evloop->modify(m_clientChannel);
}
void Connection::notifyToDisconnect()
{
	int fd = m_clientSock->getFd();
	std::function<void(int)> disconnectCallBack = m_callBack;
	m_evloop->addTask([disconnectCallBack, fd]() {
		disconnectCallBack(fd);
	});
}
Connection::~Connection()
{
	delete m_clientAddr;
	delete m_clientChannel;
	delete m_clientSock;
}