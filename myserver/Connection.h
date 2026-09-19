#ifndef __CONNECTION__
#define __CONNECTION__
#include "Buffer.h"
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
	Buffer m_inputBuffer;
	Buffer m_outputBuffer;
	std::function<void(int)> m_callBack;
	std::function<void(Connection*)> m_handleCallBack;

public:
	Connection(int fd, InetAddress* clientAddr, EventLoop* evloop);
	void recieveMessage();
	void setCallBack(std::function<void(int)> callBack);
	void setHandleCallBack(std::function<void(Connection*)> m_handleCallBack);
	Buffer* getInputBuffer();
	Buffer* getOutputBuffer();
	void sendMessage();
	void notifyToDisconnect(int cfd);
	~Connection();
};
#endif