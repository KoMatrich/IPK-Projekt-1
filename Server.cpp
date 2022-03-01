#include "Server.h"

void Server::set_port(int port)
{
	if (port > 65535)
		handle_error("invalid port number");
	if (port < 0)
		handle_error("invalid port number");

	this->port = port;
	cout << "port set to: " << port << endl;
}

Server::Server()
{
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		handle_error("socket creation");

	setsockopt(this->sock, SOL_SOCKET, SO_REUSEADDR, &this->sock_opt_val, sizeof(int));
	cout << "socket created" << endl;
}

void Server::bind_to(int port)
{
	set_port(port);

	this->addr.sin_family = AF_INET;
	this->addr.sin_addr.s_addr = htonl(INADDR_ANY);
	this->addr.sin_port = htons(this->port);

	if (bind(this->sock, (struct sockaddr*)&addr, sizeof addr) < 0)
		handle_error("socket bind");

	cout << "socket binded" << endl;
}

void Server::listen_for(int count)
{
	if (listen(this->sock, count) < 0)
		handle_error("socket listen");
	cout << "listening for " << count << " conections" << endl;
}

int Server::accept_con()
{
	struct sockaddr_in from {};
	socklen_t fromlen = sizeof(from);
	int client_id = accept(this->sock, (struct sockaddr*)&from, &fromlen);
	if (client_id < 0)
		handle_error("accept");

	client_id;

	cout << client_id << " connection, accepted" << endl;
	return client_id;
}

void Server::get_request(Client* client, Packet* request_p)
{
	array<char, 1024> buf{};

	if (read(client->get_socket(), buf.data(), buf.size()) < 0)
		handle_error("receive");

	string msg(buf.data());

	cout << client->get_socket() << " connection, data recived" << endl;

	*request_p = Packet(msg);
}

void Server::set_response(Client* client, Packet* packet_r)
{
	string msg = packet_r->encode();
	if (send(client->get_socket(), msg.data(), msg.size(), 0) < 0)
		perror("sending");

	cout << client->get_socket() << " connection, data sent" << endl;
}