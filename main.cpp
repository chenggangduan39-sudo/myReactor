#include "Channel.h"
#include "Epoll.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Logger.h"
#include "Socket.h"
#include "TcpServer.h"
int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		LogMessage("usage: ./server <ip> <port>");
		exit(-1);
	}
	TcpServer server(argv[1], argv[2]);
	server.start();
	return 0;
}