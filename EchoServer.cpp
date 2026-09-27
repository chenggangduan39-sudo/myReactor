#include "EchoServer.h"
#include "Connection.h"
#include "Logger.h"
#include <functional>
#include <string.h>
EchoServer::EchoServer(char IP[], char port[], int threadNum) : m_tcpServer(IP, port), m_workThreadPool(threadNum)
{
	m_tcpServer.setHandleCallBack(std::bind(&EchoServer::handleMessage, this, std::placeholders::_1));
}
void EchoServer::handleMessage(Connection* conn)
{
	m_workThreadPool.addTask(std::bind(&EchoServer::onMessage, this, conn));
}
bool EchoServer::parseMessage(Connection* conn, std::string& message)
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
void EchoServer::onMessage(Connection* conn)
{
	Buffer* inputBuffer = conn->getInputBuffer();
	Buffer* outputBuffer = conn->getOutputBuffer();
	while (!inputBuffer->isEmpty())
	{
		std::string message;
		if (!parseMessage(conn, message))
			break;
		LogMessage("Recieve:%s", message.data());
		outputBuffer->append(message.data(), message.size());
	}
	conn->sendMessage();
}
void EchoServer::start()
{
	m_tcpServer.start();
}