#pragma once
#include <map>

#include "Common.h"

class Response
{
private:
	int code;
	string msg;

	const map<int, string> list = {
		{200,"OK"},
		{400,"Bad Request"},
		{500,"Internal Server Error"},
		{501,"Not Implemented"}
	};
public:
	Response();
	Response(int code);
	Response(string msg);

	int get_code();
	string get_msg();
	const char* get_e_msg();
};

using response_func = Response(*)();

class ResponseHandler
{
private:
	map<string, response_func> list;
public:
	ResponseHandler()
	{
		this->list;
	}

	void set(string response, response_func func)
	{
		this->list.emplace(response, func);
	}

	Response get(string request);
};