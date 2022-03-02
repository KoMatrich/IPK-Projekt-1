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

	/// <summary>
	/// Decodes string to packet class
	/// </summary>
	/// <param name="msg"></param>
	void decode(string msg);

	/// <summary>
	/// Returns packet coded to string
	/// </summary>
	/// <returns></returns>
	string encode();

	/// <summary>
	/// Sets certain value to key,
	/// if key is not found then it will be added to end
	/// </summary>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void set(string key, string value);

	/// <summary>
	/// Gets value from given key,
	/// if key is not present returns empty string
	/// </summary>
	/// <param name="key"></param>
	/// <returns></returns>
	string get(string key);
};