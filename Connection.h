#ifndef __CONNECTION__
#define __CONNECTION__
#include <functional>
class Socket;
class InetAddress;
class Channel;
class EventLoop;
class Connection
{
private:
	Socket* m_clientSock;
	InetAddress* m_clientAddr;
	Channel* m_clientChannel;
	EventLoop* m_evloop;
	std::function<void(int)> m_callBack;

public:
	Connection(int fd, InetAddress* clientAddr, EventLoop* evloop);
	void sendMessage();
	void setCallBack(std::function<void(int)> callBack);
	void notifyToDisconnect(int cfd);
	~Connection();
};
#endif