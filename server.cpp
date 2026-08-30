#include "InetAddress.h"
#include "Logger.h"
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
void setNonBlock(int fd)
{
	int flag = fcntl(fd, F_GETFL);
	flag |= O_NONBLOCK;
	fcntl(fd, F_SETFL, flag);
}
int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		LogMessage("usage: ./server <ip> <port>");
		exit(-1);
	}
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	InetAddress serverAddr(argv[1], argv[2]);
	int res = bind(server_fd, serverAddr.getaddr(), serverAddr.getLength());
	if (res == -1)
	{
		LogMessage("Server fd bind failed");
		exit(-1);
	}
	int ret = listen(server_fd, 128);
	if (ret == -1)
		LogMessage("Server fd listen failed");
	else
		LogMessage("Server is listening");
	int epfd = epoll_create(1);
	epoll_event ev;
	ev.data.fd = server_fd;
	ev.events = EPOLLIN;
	epoll_ctl(epfd, EPOLL_CTL_ADD, server_fd, &ev);
	epoll_event events[1024];
	while (true)
	{
		int count = epoll_wait(epfd, events, sizeof(events) / sizeof(events[0]), -1);
		for (int i = 0; i < count; i++)
		{
			if (events[i].data.fd == server_fd)
			{
				InetAddress clientAddr;
				socklen_t length = clientAddr.getLength();
				int cfd = accept(server_fd, clientAddr.getaddr(), &length);
				LogMessage("Accept client:%s %d", clientAddr.getIP(), clientAddr.getPort());
				setNonBlock(cfd);
				epoll_event cev;
				cev.data.fd = cfd;
				cev.events = EPOLLIN | EPOLLET;
				epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &cev);
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
	close(server_fd);
	return 0;
}