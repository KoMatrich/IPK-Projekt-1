#pragma once

#include "Common.h"
#include "Packet.h"
#include "Client.h"

#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <array>

#include "Response.h"

using Client_handler = Packet& (*)(Packet&, Response_handler&);

class Server
{
private:
	bool running = false;
	int port, sock, sock_opt_val{ 1 };
	struct sockaddr_in addr;
	void set_port(int port);
public:
	Response_handler response_handler;

	Server();
	void bind_to(int port);
	void listen_for(int count);
	int accept_con();
	void start(Client_handler client_handler);

	Packet get_request(Client *client);
	void set_response(Client *client, Packet &packet_r);
};