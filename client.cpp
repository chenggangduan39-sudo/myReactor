#include "Logger.h"
#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		LogMessage("usage: ./client <ip> <port>");
		exit(-1);
	}
	int client_fd = socket(AF_INET, SOCK_STREAM, 0);
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
	serverAddr.sin_port = htons(atoi(argv[2]));
	int res = connect(client_fd, (sockaddr*)&serverAddr, sizeof(serverAddr));
	if (res == -1)
	{
		LogMessage("Client connect failed");
		exit(-1);
	}
	else
		LogMessage("Connect ok");
	while (true)
	{
		char buffer[1024];
		memset(buffer, 0, sizeof(buffer));
		std::cin.getline(buffer, sizeof(buffer));
		send(client_fd, buffer, sizeof(buffer), 0);
		read(client_fd, buffer, sizeof(buffer));
		LogMessage("Recieve:%s", buffer);
	}
	return 0;
}