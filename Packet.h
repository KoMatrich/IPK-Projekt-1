#pragma once

#include "Common.h"

#include <vector>
#include <map>
#include <sstream>

class Packet
{
private:
	vector<string> insertOrder;
	map<string, string> data;
public:
	Packet(string msg)
	{
		this->decode(msg);
	};
	Packet() {};

	void decode(string msg);
	string encode();

	void set(string key, string value);
	string get(string key);
};