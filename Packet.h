#pragma once

#include "Common.h"

#include <vector>
#include <map>
#include <sstream>

/// <summary>
/// Stores data of packet in map and their order,
/// in which they have been first inicialized
/// </summary>
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
	/// Decodes string and packet class values
	/// </summary>
	/// <param name="msg"></param>
	void decode(string msg);

	/// <summary>
	/// Encodes packet class to string
	/// </summary>
	/// <returns>
	/// packet coded to string
	/// </returns>
	string encode();

	/// <summary>
	/// Sets certain value to key
	/// new keys will be added to end
	/// (order sensitive)
	/// </summary>
	/// <param name="key"></param>
	/// <param name="value"></param>
	void set(string key, string value);

	/// <summary>
	/// Gets value from given key,
	/// </summary>
	/// <param name="key"></param>
	/// <returns>
	/// value of given key if found,
	/// else empty string
	/// </returns>
	string get(string key);
};