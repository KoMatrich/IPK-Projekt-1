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

/// <summary>
/// definition of client handeling function that has to generate response packet based on request packet
/// </summary>
using Client_handler = Packet& (*)(Packet&, Response_handler&);

/// <summary>
/// HTTP Server that is using sockets
/// </summary>
class Server
{
private:
	bool running = false;
	int port, sock = -1;
	int sock_opt_val{ 1 };
	struct sockaddr_in addr;
	void set_port(int port);

	/// <summary>
	/// return request packet
	/// used internaly
	/// </summary>
	Packet get_request(Client* client);
	/// <summary>
	/// sends request packet
	/// used internaly
	/// </summary>
	void set_response(Client* client, Packet& packet_r);
public:
	Response_handler response_handler;

	Server();
	void bind_to(int port);
	void listen_for(int count);
	int accept_con();

	/// <summary>
	/// starts server on another thread if DEBUG is not defined
	/// </summary>
	void start(Client_handler client_handler);
	
	/// <summary>
	/// prints msg as server
	/// </summary>
	void print(const char* fmt, ...);
};