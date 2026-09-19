#include "InetAddress.h"
#include <arpa/inet.h>
#include <stdlib.h>
InetAddress::InetAddress(char IP[], char port[]) : m_addrLength(sizeof(sockaddr))
{
	m_address.sin_family = AF_INET;
	m_address.sin_addr.s_addr = inet_addr(IP);
	m_address.sin_port = htons(atoi(port));
}
InetAddress::InetAddress() : m_addrLength(sizeof(sockaddr))
{
}
sockaddr* InetAddress::getaddr()
{
	return (sockaddr*)&m_address;
}
socklen_t* InetAddress::getLengthAddr()
{
	return &m_addrLength;
}
char* InetAddress::getIP()
{
	return inet_ntoa(m_address.sin_addr);
}
int InetAddress::getPort()
{
	return ntohs(m_address.sin_port);
}
socklen_t InetAddress::getLength()
{
	return m_addrLength;
}
InetAddress::~InetAddress()
{
}