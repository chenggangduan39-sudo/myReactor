#include "TcpServer.h"
#include "Acceptor.h"
#include "Connection.h"
#include "InetAddress.h"
#include "Logger.h"
#include "Socket.h"
TcpServer::TcpServer(char IP[], char port[], int threadNum) : m_threadNum(threadNum)
{
	m_acceptor = new Acceptor(IP, port, &m_mainLoop);
	m_threadPool = new ThreadPool(m_threadNum, "IO");
	if (threadNum == 0)
		LogMessage("TcpServer::TcpServer():No threads were created");
	for (int i = 0; i < m_threadNum; i++)
	{
		m_evloops.emplace_back(new EventLoop);
		m_threadPool->addTask(std::bind(&EventLoop::run, m_evloops[i]));
	}
	m_acceptor->setCallBack(std::bind(&TcpServer::createConnection, this, std::placeholders::_1, std::placeholders::_2));
}
void TcpServer::start()
{
	m_mainLoop.run();
}
void TcpServer::createConnection(int fd, InetAddress* clientAddr)
{
	sharedPtrConn connection = nullptr;
	if (m_threadNum > 0)
		connection = std::make_shared<Connection>(fd, clientAddr, m_evloops[fd % m_threadNum]);
	else
		connection = std::make_shared<Connection>(fd, clientAddr, &m_mainLoop);
	connection->setCallBack(std::bind(&TcpServer::disconnect, this, std::placeholders::_1));
	connection->setHandleCallBack(m_handleCallBack);
	m_connlist.insert({fd, connection});
}
void TcpServer::disconnect(int cfd)
{
	m_connlist.erase(cfd);
}
void TcpServer::setHandleCallBack(std::function<void(sharedPtrConn)> handleCallBack)
{
	m_handleCallBack = handleCallBack;
}