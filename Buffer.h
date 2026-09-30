#ifndef __BUFFER__
#define __BUFFER__
#include <string>
class Buffer
{
private:
	std::string m_buffer;

public:
	void append(const char* data, int size);
	void erase(int pos, int count);
	const char* data();
	size_t size();
	bool isEmpty();
};
#endif