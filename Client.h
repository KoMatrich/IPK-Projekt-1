#pragma once

#include "Common.h"
#include <unistd.h>

class Client
{
private:
	int connfd;
public:
	Client(int id)
	{
		this->connfd = id;
		this->print("created");
	}
	~Client()
	{
		this->print("closed");
		close(this->connfd);
	}

	int get_socket()
	{
		return connfd;
	}

	void print(const char* fmt, ...);
};