#ifndef __INETADDRESS__
#define __INETADDRESS__
#include <netinet/in.h>
#include <string>
class InetAddress
{
private:
	sockaddr_in m_address;
	socklen_t m_addrLength;

public:
	InetAddress(char IP[], char port[]);
	InetAddress();
	sockaddr* getaddr();
	socklen_t getLength();
	char* getIP();
	int getPort();
	~InetAddress();
};
#endif