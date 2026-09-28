#ifndef __CONNECTION__
#define __CONNECTION__
#include "Buffer.h"
#include <functional>
#include <memory>
class Connection;
using sharedPtrConn = std::shared_ptr<Connection>;
class Socket;
class InetAddress;
class Channel;
class EventLoop;
class Connection : public std::enable_shared_from_this<Connection>
{
private:
	Socket* m_clientSock;
	InetAddress* m_clientAddr;
	Channel* m_clientChannel;
	EventLoop* m_evloop;
	Buffer m_inputBuffer;
	Buffer m_outputBuffer;
	std::function<void(int)> m_callBack;
	std::function<void(sharedPtrConn)> m_handleCallBack;

public:
	Connection(int fd, InetAddress* clientAddr, EventLoop* evloop);
	void recieveMessage();
	void setCallBack(std::function<void(int)> callBack);
	void setHandleCallBack(std::function<void(sharedPtrConn)> m_handleCallBack);
	Buffer* getInputBuffer();
	Buffer* getOutputBuffer();
	void sendMessage();
	void notifyToDisconnect(int cfd);
	~Connection();
};
#endif