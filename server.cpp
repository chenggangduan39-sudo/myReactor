#include "Epoll.h"
#include "InetAddress.h"
#include "Logger.h"
#include "Socket.h"
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		LogMessage("usage: ./server <ip> <port>");
		exit(-1);
	}
	InetAddress serverAddr(argv[1], argv[2]);
	Socket serverSock;
	int res = serverSock.socketBind(serverAddr);
	if (res == -1)
	{
		LogMessage("Server fd bind failed");
		exit(-1);
	}
	int ret = serverSock.socketListen();
	if (ret == -1)
		LogMessage("Server fd listen failed");
	else
		LogMessage("Server is listening");
	Epoll epoll;
	epoll.epollAdd(serverSock.getFd());
	while (true)
	{
		std::vector<epoll_event> events = epoll.wait();
		for (auto event : events)
		{
			if (event.data.fd == serverSock.getFd())
			{
				InetAddress clientAddr;
				socklen_t length = clientAddr.getLength();
				int cfd = accept(serverSock.getFd(), clientAddr.getaddr(), &length);
				Socket* clientSock = new Socket(cfd);
				LogMessage("Accept client:%s %d", clientAddr.getIP(), clientAddr.getPort());
				clientSock->setNonBlock();
				epoll.epollAdd(clientSock->getFd());
			}
			else
			{
				char buffer[1024];
				memset(buffer, 0, sizeof(buffer));
				int num = 0;
				while (num = read(event.data.fd, buffer, sizeof(buffer)))
				{
					if (num <= 0)
						break;
					LogMessage(buffer);
					send(event.data.fd, buffer, sizeof(buffer), 0);
				}
				if (num == 0)
				{
					LogMessage("Disconnected");
					epoll.epollRemove(event.data.fd);
				}
				else if ((num == -1 && errno == EAGAIN) || (num == -1 && errno == EWOULDBLOCK))
					break;
			}
		}
	}
	return 0;
}