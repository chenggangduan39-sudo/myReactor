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
	int m_workerThreadNum;
	int m_subReactorNum;
	ThreadPool m_workThreadPool;

public:
	EchoServer(char IP[], char port[], int workerThreadNum = 0, int subReactorNum = 0);
	void handleMessage(sharedPtrConn conn);
	bool parseMessage(sharedPtrConn conn, std::string& message);
	void handleBusiness(sharedPtrConn conn, std::string message);
	void start();
};
#endif