#include "IPK.h"

class Response
{
private:
	int code;
	string msg;

	const map<int, string> list = {
		{200,"OK"},
		{400,"Bad Request"},
		{500,"Internal Server Error"},
		{501,"Not Implemented"}
	};
public:
	Response();
	Response(int code);
	Response(string msg);

	int get_code();
	string get_msg();
	const char* get_e_msg();
};

Response::Response()
{
	this->code = 501;
	this->msg = this->get_e_msg();
}

Response::Response(int code)
{
	this->code = code;
	this->msg = this->get_e_msg();
}

Response::Response(string msg)
{
	this->code = 200;
	this->msg = msg;
}

int Response::get_code()
{
	return this->code;
}

string Response::get_msg()
{
	return this->msg;
}

const char* Response::get_e_msg()
{
	if (list.find(code) == list.end()) {
		return "Uknown Return Code";
	} else {
		return list.at(code).data();
	}
}


using response_func = Response(*)();

class ResponseHandler
{
private:
	map<string, response_func> list;
public:
	ResponseHandler()
	{
		this->list;
	}

	void set(string response, response_func func)
	{
		this->list.emplace(response, func);
	}

	Response get(string request);
};

Response ResponseHandler::get(string request)
{
	if (this->list.find(request) == this->list.end()) {
		return Response();
	}

	return this->list.at(request)();
}


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

void Packet::decode(string msg)
{
	string line;
	stringstream msg_stream(msg.c_str());
	bool first = true;
	while (getline(msg_stream, line)) {
		auto pos = 0;
		if (first) {
			first = false;
			pos = line.find(' ', line.size());
		} else
			pos = line.find(": ", line.size());

		if (pos == line.size())
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


class Client
{
private:
	int connfd;
public:
	Client(int id)
	{
		this->connfd = id;
	}

	int get_socket()
	{
		return connfd;
	}
};


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


Response r_hostname()
{
	array<char, 128> buf{};

	gethostname(buf.data(), buf.max_size());
	buf.back() = '\0';

	return Response(buf.data());
}

Response r_cpu_name()
{
	const char* command("cat /proc/cpuinfo | grep 'model name' | head -n 1 | awk -F ':' '{ print $2 }' | sed -e 's/^[[:space:]]*//'");
	array<char, 128> buf{};

	FILE* pipe = popen(command, "r");
	if (!pipe) {
		std::cerr << "Couldn't start shell command.\n";
		return Response(500);
	}

	fgets(buf.data(), 128, pipe);
	pclose(pipe);

	//removes new line character
	for (int i = 0; i < buf.size(); i++) {
		if (buf.at(i) == '\n')
			buf.at(i) = '\0';
	}

	return Response(buf.data());
}

bool get_system_times(uint64_t* idlelast, uint64_t* systemlast, uint64_t* virtuallast)
{
	// Different kernels hold up to 10 values
	long long unsigned int usertime, nicetime, systemtime, idletime;
	long long unsigned int ioWait, irq, softIrq, steal, guest, guestnice;
	ioWait = irq = softIrq = steal = guest = guestnice = 0;

	// Iterate over all CPU's and pipe the sum
	const char* shcommand("cat /proc/stat | grep 'cpu' | sed 's/  / /g' | awk -F' ' '{s2+=$2;s3+=$3;s4+=$4;s5+=$5;s6+=$6;s7+=$7;s8+=$8;s9+=$9;s10+=$10;s11+=$11} END {print s2,s3,s4,s5,s6,s7,s8,s9,s10,s11}'");

	FILE* pipe = popen(shcommand, "r");
	if (!pipe) {
		return false;
	}

	fscanf(pipe, "%16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu %16llu", &usertime, &nicetime,
		   &systemtime, &idletime, &ioWait, &irq, &softIrq, &steal, &guest, &guestnice);

	pclose(pipe);

	// Guest time is already accounted in usertime
	usertime = usertime - guest;
	nicetime = nicetime - guestnice;

	// Fields existing on kernels >= 2.6
	// (and RHEL's patched kernel 2.4...)
	*idlelast = idletime + ioWait;
	*systemlast = systemtime + irq + softIrq;
	*virtuallast = guest + guestnice;

	return true;
}

Response r_load()
{
	double usagepercent = -1;
	uint64_t idlelast, systemlast, virtuallast;
	uint64_t idlenow, systemnow, virtualnow;

	if (!get_system_times(&idlelast, &systemlast, &virtuallast)) {
		perror("Shell CMD failed");
		return Response(500);
	}

	using namespace std::this_thread;     // sleep_for, sleep_until
	using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
	using std::chrono::system_clock;
	sleep_for(2s);

	if (!get_system_times(&idlenow, &systemnow, &virtualnow)) {
		perror("Shell CMD failed");
		return Response(500);
	}

	if (idlelast > idlenow || systemlast > systemnow || virtuallast > virtualnow) {
		perror("Time counters over-flow");
		return Response(500);
	} else {
		uint64_t dsystem = systemnow - systemlast;
		uint64_t dvirtual = virtualnow - virtuallast;
		uint64_t didle = idlenow - idlelast;
		uint64_t load = round(100.0 * (dsystem + dvirtual) / (dsystem + dvirtual + didle));
		return Response(to_string(load) + '%');
	}
}

#define DEBUG
int main(int argc, char* argv[])
{
	if (argc == 1) {
		cerr << "No port number passed." << endl;
		handle_error("Usage: ./hinfosvc PORT");
	}

	if (argc > 2)
		handle_error("Too many aguments.");

	string portString = argv[1];
	if (!isDigit(portString)) {
		cerr << "\"" << portString << "\" is not valid port number." << endl;
		return EXIT_FAILURE;
	}
	int portNumber = atoi(portString.data());

	Server server;
	server.bind_to(portNumber);

	ResponseHandler responses;
	responses.set("/hostname", r_hostname);
	responses.set("/cpu-name", r_cpu_name);
	responses.set("/load", r_load);

	server.listen_for(5);
	while (true) {
		Client client(server.accept_con());

#ifndef DEBUG
		int pid = fork();
		if (pid < 0) {
			handle_error("Fork failed");
		}

		if (pid == 0) {
#endif // DEBUG

			Packet request_p, response_p;
			server.get_request(&client, &request_p);

			string get_req = request_p.get("GET");
			get_req = get_req.substr(0, get_req.find(" "));

			Response response = responses.get(get_req);

			string head = to_string(response.get_code()) + " " + response.get_e_msg();

			response_p.set("HTTP/1.1", head);
			response_p.set("Content-type", "text/html");
			if (response.get_code() < 500)
				response_p.set("Body", response.get_msg());
			else
				response_p.set("Body", head);

			server.set_response(&client, &response_p);

			close(client.get_socket());
			cout << client.get_socket() << " connection, terminated" << endl;

#ifndef DEBUG
			exit(0);
		}
#endif // DEBUG
	}
}