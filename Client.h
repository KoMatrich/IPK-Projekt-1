#pragma once

#include "Common.h"
#include <unistd.h>

class Client
{
private:
	int connfd;
public:
	/// <summary>
	/// initicialize client class
	/// </summary>
	/// <param name="id">
	/// socket id
	/// </param>
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

	/// <returns>
	/// socket id
	///</returns>
	int get_socket()
	{
		return connfd;
	}

	/// <summary>
	/// prints msg as client
	/// </summary>
	void print(const char* fmt, ...);
};