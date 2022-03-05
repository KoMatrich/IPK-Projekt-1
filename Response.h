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
	//default response
	Response();
	//correct response
	Response(int code);
	//correct response with custom msg
	Response(string msg);

	/// <returns>
	/// code of response
	///</returns>
	int get_code();

	/// <returns>
	/// response message
	///	</returns>
	string get_msg();

	/// <returns>
    /// message generated from return code in response,
    /// if code is not found returns "Uknown Return Code"
    /// </returns>
	const char* get_e_msg();
};

//definition of Response function
using response_func = Response(*)();

/// <summary>
/// Acts as operation storage of response functions
/// </summary>
class Response_handler
{
private:
	map<string, response_func> list;
public:
	void set(string response, response_func func)
	{
		this->list.emplace(response, func);
	}

	/// <returns>
    /// Response to request, if request is valid
    /// else returns default request (empty)
    /// </returns>
	Response get(string request);
};