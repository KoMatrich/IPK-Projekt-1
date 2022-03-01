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
	}

	int get_socket()
	{
		return connfd;
	}
};