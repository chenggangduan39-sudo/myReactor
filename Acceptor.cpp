#include "Acceptor.h"
#include "Channel.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Logger.h"
#include "Socket.h"
#include <sys/socket.h>
Acceptor::Acceptor(char IP[], char port[], EventLoop* evloop) : m_evloop(evloop)
{
	m_serverSock = new Socket;
	m_serverAddr = new InetAddress(IP, port);
	int res = m_serverSock->socketBind(*m_serverAddr);
	if (res == -1)
	{
		LogMessage("Server fd bind failed");
		exit(-1);
	}
	int ret = m_serverSock->socketListen();
	if (ret == -1)
		LogMessage("Server fd listen failed");
	else
		LogMessage("Server is listening");
	m_serverChannel = new Channel(m_serverSock->getFd());
	m_serverChannel->enableReading();
	m_serverChannel->setReadCallBack(std::bind(&Acceptor::acceptClient, this));
	m_evloop->add(m_serverChannel);
}
void Acceptor::acceptClient()
{
	InetAddress* clientAddr = new InetAddress;
	int cfd = accept(m_serverSock->getFd(), clientAddr->getaddr(), clientAddr->getLengthAddr());
	m_CallBack(cfd, clientAddr);
}
void Acceptor::setCallBack(std::function<void(int fd, InetAddress* clientAddr)> callBack)
{
	m_CallBack = callBack;
}
