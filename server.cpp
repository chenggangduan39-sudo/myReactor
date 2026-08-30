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
	int epfd = epoll_create(1);
	epoll_event ev;
	ev.data.fd = serverSock.getFd();
	ev.events = EPOLLIN;
	epoll_ctl(epfd, EPOLL_CTL_ADD, serverSock.getFd(), &ev);
	epoll_event events[1024];
	while (true)
	{
		int count = epoll_wait(epfd, events, sizeof(events) / sizeof(events[0]), -1);
		for (int i = 0; i < count; i++)
		{
			if (events[i].data.fd == serverSock.getFd())
			{
				InetAddress clientAddr;
				socklen_t length = clientAddr.getLength();
				int cfd = accept(serverSock.getFd(), clientAddr.getaddr(), &length);
				Socket* clientSock = new Socket(cfd);
				LogMessage("Accept client:%s %d", clientAddr.getIP(), clientAddr.getPort());
				clientSock->setNonBlock();
				epoll_event cev;
				cev.data.fd = clientSock->getFd();
				cev.events = EPOLLIN | EPOLLET;
				epoll_ctl(epfd, EPOLL_CTL_ADD, clientSock->getFd(), &cev);
			}
			else
			{
				char buffer[1024];
				memset(buffer, 0, sizeof(buffer));
				int num = 0;
				while (num = read(events[i].data.fd, buffer, sizeof(buffer)))
				{
					if (num <= 0)
						break;
					LogMessage(buffer);
					send(events[i].data.fd, buffer, sizeof(buffer), 0);
				}
				if (num == 0)
				{
					LogMessage("Disconnected");
					epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, nullptr);
					close(events[i].data.fd);
				}
				else if ((num == -1 && errno == EAGAIN) || (num == -1 && errno == EWOULDBLOCK))
					break;
			}
		}
	}
	return 0;
}