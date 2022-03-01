#pragma once

#include "Common.h"

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

	int get_socket()
	{
		return connfd;
	}

	void print(const char* fmt, ...);
};