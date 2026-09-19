#include "TcpServer.h"
#include "Acceptor.h"
#include "Connection.h"
#include "InetAddress.h"
#include "Logger.h"
#include "Socket.h"
TcpServer::TcpServer(char IP[], char port[])
{
	m_acceptor = new Acceptor(IP, port, &m_evloop);
	m_acceptor->setCallBack(
	    std::bind(&TcpServer::createConnection, this, std::placeholders::_1, std::placeholders::_2, &m_evloop));
}
void TcpServer::start()
{
	m_evloop.run();
}
void TcpServer::createConnection(int fd, InetAddress* clientAddr, EventLoop* evloop)
{
	Connection* connection = new Connection(fd, clientAddr, evloop);
	connection->setCallBack(std::bind(&TcpServer::disconnect, this, std::placeholders::_1));
	connection->setHandleCallBack(m_handleCallBack);
	m_connlist.insert({fd, connection});
}
void TcpServer::disconnect(int cfd)
{
	delete m_connlist[cfd];
	m_connlist.erase(cfd);
}
void TcpServer::setHandleCallBack(std::function<void(Connection*)> handleCallBack)
{
	m_handleCallBack = handleCallBack;
}