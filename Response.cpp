#include "Response.h"

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

const char* Response::get_e_msg()
{
	if (list.find(code) == list.end()) {
		return "Uknown Return Code";
	} else {
		return list.at(code).data();
	}
}

Response Response_handler::get(string request)
{
	if (this->list.find(request) == this->list.end()) {
		return Response();
	}

	return this->list.at(request)();
}
