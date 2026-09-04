#ifndef __ACCEPTOR__
#define __ACCEPTOR__
#include <functional>
class Socket;
class InetAddress;
class EventLoop;
class Channel;
class Acceptor
{
private:
	Socket* m_serverSock;
	InetAddress* m_serverAddr;
	Channel* m_serverChannel;
	EventLoop* m_evloop;
	std::function<void(int fd, InetAddress* clientAddr)> m_CallBack;

public:
	Acceptor(char IP[], char port[], EventLoop* evloop);
	void acceptClient();
	void setCallBack(std::function<void(int fd, InetAddress* clientAddr)> callBack);
};
#endif