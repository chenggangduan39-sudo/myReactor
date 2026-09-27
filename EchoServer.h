#ifndef __ECHOSERVER__
#define __ECHOSERVER__
#include "TcpServer.h"
#include "ThreadPool.h"
#include <string>
class Connection;
class EchoServer
{
private:
	TcpServer m_tcpServer;
	ThreadPool m_workThreadPool;

public:
	EchoServer(char IP[], char port[], int threadNum = 8);
	void handleMessage(Connection* conn);
	bool parseMessage(Connection* conn, std::string& message);
	void onMessage(Connection* conn);
	void start();
};
#endif