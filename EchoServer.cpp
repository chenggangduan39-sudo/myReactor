#include "EchoServer.h"
#include "Connection.h"
#include "Logger.h"
#include <functional>
#include <string.h>
#include <sys/syscall.h>
EchoServer::EchoServer(char IP[], char port[], int workerThreadNum, int subReactorNum)
    : m_tcpServer(IP, port, subReactorNum), m_workerThreadNum(workerThreadNum),
      m_workThreadPool(workerThreadNum, "WORKER")
{
	if (m_workerThreadNum == 0)
		LogMessage("EchoServer::EchoServer():No threads were created");
	m_tcpServer.setHandleCallBack(std::bind(&EchoServer::handleMessage, this, std::placeholders::_1));
}
void EchoServer::handleMessage(sharedPtrConn conn)
{
	LogMessage("EchoServer::handleMessage() thread is %d", syscall(SYS_gettid));
	if (m_workerThreadNum > 0)
		m_workThreadPool.addTask(std::bind(&EchoServer::handleBusiness, this, conn));
	else
		handleBusiness(conn);
}
bool EchoServer::parseMessage(sharedPtrConn conn, std::string& message)
{
	Buffer* inputBuffer = conn->getInputBuffer();
	if (inputBuffer->isEmpty())
		return false;
	else if (inputBuffer->size() <= 4)
		return false;
	int length;
	memcpy(&length, inputBuffer->data(), 4);
	if (inputBuffer->size() - 4 < length)
		return false;
	else
	{
		message = std::string(inputBuffer->data() + 4, length);
		inputBuffer->erase(0, length + 4);
		return true;
	}
}
void EchoServer::handleBusiness(sharedPtrConn conn)
{
	LogMessage("EchoServer::handleBusiness() thread is %d", syscall(SYS_gettid));
	Buffer* inputBuffer = conn->getInputBuffer();
	std::string message;
	while (!inputBuffer->isEmpty())
	{
		if (!parseMessage(conn, message))
			break;
		LogMessage("Recieve:%s", message.data());
		// sleep(2);
		conn->sendMessage(message);
	}
}
void EchoServer::start()
{
	m_tcpServer.start();
}