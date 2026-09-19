#include "EchoServer.h"
#include "Logger.h"
int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		LogMessage("usage: ./server <ip> <port>");
		exit(-1);
	}
	EchoServer server(argv[1], argv[2]);
	server.start();
	return 0;
}