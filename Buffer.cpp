#include "Buffer.h"
void Buffer::append(const char* data, int size)
{
	m_buffer.append(data, size);
}
void Buffer::erase(int pos, int count)
{
	m_buffer.erase(pos, count);
}
char* Buffer::data()
{
	return m_buffer.data();
}
size_t Buffer::size()
{
	return m_buffer.size();
}
bool Buffer::isEmpty()
{
	return m_buffer.empty();
}