#include "Packet.h"

void Packet::decode(string msg)
{
	string line;
	stringstream msg_stream(msg.c_str());
	bool first = true;
	while (getline(msg_stream, line)) {
		auto pos = 0;
		if (first) {
			first = false;
			pos = line.find(' ');
		} else
			pos = line.find(": ");

		if (pos < 0 || pos >= line.size())
			perror("Decode failed to find separator");

		string key = line.substr(0, pos);
		string value;
		if (key == "\r") {
			key = "Body";
			value = "";
			this->set(key, value);
			break;
		} else {
			value = line.substr(pos + 1, line.length() - pos - 2);
			this->set(key, value);
		}
	}
}

string Packet::encode()
{
	string msg;
	for (string key : this->insertOrder) {
		if (key == "Body" || key == "Content-Lenght")
			continue;

		msg.append(key);
		if (key == this->insertOrder.front())
			msg.append(" ");
		else
			msg.append(":");
		msg.append(this->data.at(key));
		msg.append("\n");
	}

	msg.append("Content-Lenght: ");
	int len = this->get("Body").size();
	msg.append(to_string(len));
	msg.append("\n");

	msg.append("\n");
	msg.append(this->get("Body"));
	msg.append("\n");
	return msg;
}

void Packet::set(string key, string value)
{
	if (this->data.find(key) == this->data.end())
		insertOrder.push_back(key);
	this->data.emplace(key, value);
}

string Packet::get(string key)
{
	if (this->data.find(key) == this->data.end())
		return "";
	else
		return this->data.at(key);
}