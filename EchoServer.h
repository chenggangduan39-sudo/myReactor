#ifndef __ECHOSERVER__
#define __ECHOSERVER__
#include "TcpServer.h"
#include <string>
class Connection;
class EchoServer
{
private:
	TcpServer m_tcpServer;

public:
	EchoServer(char IP[], char port[]);
	void handleMessage(Connection* conn);
	bool parseMessage(Connection* conn, std::string& message);
	void start();
};
#endif