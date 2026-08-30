#ifndef __SOCKET__
#define __SOCKET__
#include "InetAddress.h"
#include <sys/socket.h>
#include <unistd.h>
class Socket
{
private:
	int m_fd;

public:
	Socket();
	Socket(int fd);
	int socketBind(InetAddress address);
	int socketListen(int num = 128);
	int getFd();
	void setNonBlock();
	~Socket();
};
#endif