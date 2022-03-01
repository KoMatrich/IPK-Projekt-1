#include "Client.h"
#include <cstdarg>

void Client::print(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	printf("%d connection ", this->get_socket());
	printf(fmt, args);
	printf("\n");
	va_end(args);
}
