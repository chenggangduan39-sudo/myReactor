#include "Socket.h"
#include <fcntl.h>
#include <sys/socket.h>
Socket::Socket() : m_fd(socket(AF_INET, SOCK_STREAM, 0))
{
}
Socket::Socket(int fd) : m_fd(fd)
{
}
int Socket::socketBind(InetAddress address)
{
	return bind(m_fd, address.getaddr(), address.getLength());
}
int Socket::socketListen(int num)
{
	return listen(m_fd, 128);
}
int Socket::getFd()
{
	return m_fd;
}
void Socket::setNonBlock()
{
	int flag = fcntl(m_fd, F_GETFL);
	flag |= O_NONBLOCK;
	fcntl(m_fd, F_SETFL, flag);
}
Socket::~Socket()
{
	close(m_fd);
}