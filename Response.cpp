#include "Response.h"

//default response
Response::Response()
{
	this->code = 501;
	this->msg = this->get_e_msg();
}

Response::Response(int code)
{
	this->code = code;
	this->msg = this->get_e_msg();
}

//correct response with custom msg
Response::Response(string msg)
{
	this->code = 200;
	this->msg = msg;
}

int Response::get_code()
{
	return this->code;
}

string Response::get_msg()
{
	return this->msg;
}

/// <returns>
/// message generated from return code in response,
/// if code is not found returns "Uknown Return Code"
/// </returns>
const char* Response::get_e_msg()
{
	if (list.find(code) == list.end()) {
		return "Uknown Return Code";
	} else {
		return list.at(code).data();
	}
}

/// <returns>
/// Returns response to request, if request is valid
/// else returns default request (empty)
/// </returns>
Response Response_handler::get(string request)
{
	if (this->list.find(request) == this->list.end()) {
		return Response();
	}

	return this->list.at(request)();
}
