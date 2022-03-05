#include "Server.h"

Server::Server()
{
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		exit_errno("socket creation");

	setsockopt(this->sock, SOL_SOCKET, SO_REUSEADDR, &this->sock_opt_val, sizeof(int));
	print("socket created");
}

void Server::set_port(int port)
{
	if (port > 65535)
		exit_error("invalid port number");
	if (port < 0)
		exit_error("invalid port number");

	this->port = port;
	print("port set to: %d", port);
}

void Server::bind_to(int port)
{
	set_port(port);

	this->addr.sin_family = AF_INET;
	this->addr.sin_addr.s_addr = htonl(INADDR_ANY);
	this->addr.sin_port = htons(this->port);

	if (bind(this->sock, (struct sockaddr*)&addr, sizeof addr) < 0)
		exit_errno("socket bind");

	print("socket binded");
}

void Server::listen_for(int count)
{
	if (this->port < 0)
		exit_error("Port hasnt been bound.");
	if (listen(this->sock, count) < 0)
		exit_errno("socket listen");
	print("listening for %d conections",count);
}

int Server::accept_con()
{
	struct sockaddr_in from {};
	socklen_t fromlen = sizeof(from);
	int client_id = accept(this->sock, (struct sockaddr*)&from, &fromlen);
	if (client_id < 0)
		perror("accept");
	return client_id;
}

void Server::start(Client_handler client_handler)
{
	if (this->running) {
		print_error("Server is already running!");
		return;
	}

	this->running = true;
	while (this->running) {
		Client *client = new Client(this->accept_con());
#ifndef DEBUG
		int pid = fork();
		if (pid < 0) {
			exit_errno("fork");
		}
		if (pid == 0) {
#endif // DEBUG
			Packet request_p = this->get_request(client);

			Packet response_p = client_handler(request_p, this->response_handler);

			this->set_response(client, response_p);

			delete client;
#ifndef DEBUG
			exit(0);
		}
		delete client;
#endif // DEBUG
	}
}

void Server::print(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	printf("server: ");
	vprintf(fmt, args);
	printf("\n");
	va_end(args);
}


Packet Server::get_request(Client *client)
{
	array<char, 1024> buf{};

	if (read(client->get_socket(), buf.data(), buf.size()) < 0)
		perror("receive");

	string msg(buf.data());

	client->print("data recived");

	return Packet(msg);
}

void Server::set_response(Client *client, Packet &packet_r)
{
	string msg = packet_r.encode();
	if (send(client->get_socket(), msg.data(), msg.size(), 0) < 0)
		perror("sending");

	client->print("data sent");
}