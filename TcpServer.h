#ifndef __TCPSERVER__
#define __TCPSERVER__
#include "EventLoop.h"
#include "ThreadPool.h"
#include <functional>
#include <unordered_map>
#include <vector>
class Acceptor;
class Socket;
class InetAddress;
class Channel;
class Connection;
class TcpServer
{
private:
	EventLoop m_mainLoop;
	Acceptor* m_acceptor;
	std::vector<EventLoop*> m_evloops;
	ThreadPool* m_threadPool;
	int m_threadNum;
	std::unordered_map<int, Connection*> m_connlist;
	std::function<void(Connection*)> m_handleCallBack;

public:
	TcpServer(char IP[], char port[], int threadNum = 8);
	void start();
	void createConnection(int fd, InetAddress* clientAddr);
	void disconnect(int cfd);
	void setHandleCallBack(std::function<void(Connection*)> handleCallBack);
};
#endif