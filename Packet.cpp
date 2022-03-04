#include "Packet.h"

void Packet::decode(string msg)
{
	string line;
	stringstream msg_stream(msg.c_str());
	bool first = true;
	while (getline(msg_stream, line)) {
		//KEY+SEPARATOR+value
		auto pos = 0;
		if (first) {
			first = false;
			pos = line.find(' ');
			//HEAD/GET/POST+' '+value
		} else {
			pos = line.find(": ");
			//ATRIBUTE+": "+value
		}
		string key = line.substr(0, pos);
		string value;

		if (key == "\r") {
			//Body begins we dont need another data
			key = "Body";
			value = "";
			this->set(key, value);
			break;
		} else {
			//NOT BODY PARTS
			if (pos < 0 || pos >= line.size()) {
				print_error("Decode failed to find separator");
				return;
			}
			value = line.substr(pos + 1, line.length() - pos - 2);
			this->set(key, value);
		}
	}
}

string Packet::encode()
{
	string msg;
	for (string key : this->insertOrder) {
		//skips Body and Content-Lenght
		if (key == "Body" || key == "Content-Lenght")
			continue;

		//adds everything else in order to message in proper format
		msg.append(key);
		if (key == this->insertOrder.front())
			msg.append(" ");
		else
			msg.append(":");
		msg.append(this->data.at(key));
		msg.append("\n");
	}

	//all atributes have been added to message
	//last thing missing is content-lenght and body
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