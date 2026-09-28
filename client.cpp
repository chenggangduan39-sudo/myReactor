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
	// while (true)
	// {
	// 	char buffer[1024];
	// 	memset(buffer, 0, sizeof(buffer));
	// 	std::cin.getline(buffer, sizeof(buffer));
	// 	send(client_fd, buffer, sizeof(buffer), 0);
	// 	read(client_fd, buffer, sizeof(buffer));
	// 	LogMessage("Recieve:%s", buffer);
	// }
	for (int i = 0; i < 1; i++)
	{
		char tmp[1024];
		memset(tmp, 0, sizeof(tmp));
		snprintf(tmp, sizeof(tmp), "这是第%d个超级女生", i);
		int length = strlen(tmp);
		int res = 0;
		int offset = 0;
		char message[1024];
		memset(message, 0, sizeof(message));
		memcpy(message, &length, 4);
		memcpy(message + 4, tmp, length);
		length += 4;
		while (length > 0)
		{
			res = send(client_fd, message + offset, length, 0);
			if (res > 0)
			{
				length -= res;
				offset += res;
			}
		}
	}
	// sleep(1);
	// return 0;
	while (true)
	{
		char buffer[1024];
		memset(buffer, 0, sizeof(buffer));
		int num = read(client_fd, buffer, sizeof(buffer));
		if (num > 0)
			LogMessage("Reply:%s", buffer);
		else if (num == 0)
		{
			LogMessage("Disconnect");
			break;
		}
	}
	return 0;
}