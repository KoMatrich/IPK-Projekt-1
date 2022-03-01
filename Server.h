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

class Server
{
private:
	int port, sock, sock_opt_val{ 1 };
	struct sockaddr_in addr;
	void set_port(int port);
public:
	Server();
	void bind_to(int port);
	void listen_for(int count);
	int accept_con();

	void get_request(Client* client, Packet* request_p);
	void set_response(Client* client, Packet* packet_r);
};