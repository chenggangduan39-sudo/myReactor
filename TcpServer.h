#ifndef __TCPSERVER__
#define __TCPSERVER__
#include "EventLoop.h"
#include <unordered_map>
class Acceptor;
class Socket;
class InetAddress;
class Channel;
class Connection;
class TcpServer
{
private:
	EventLoop m_evloop;
	Acceptor* m_acceptor;
	std::unordered_map<int, Connection*> m_connlist;

public:
	TcpServer(char IP[], char port[]);
	void start();
	void createConnection(int fd, InetAddress* clientAddr, EventLoop* evloop);
	void disconnect(int cfd);
};
#endif