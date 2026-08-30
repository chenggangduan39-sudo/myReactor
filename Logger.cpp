#include <iostream>
#include <stdarg.h>
#include <string.h>
void LogMessage(char format[], ...)
{
	va_list args;
	va_start(args, format);
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	vsnprintf(buffer, sizeof(buffer), format, args);
	std::cout << buffer << std::endl;
	va_end(args);
}