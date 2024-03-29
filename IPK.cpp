﻿#include "Common.h"
#include "Response.h"
#include "server.h"

#include <unistd.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <array>
#include <math.h>

/// <summary>
/// function that handles "GET /hostname" request
/// </summary>
/// <returns>
/// hostname
/// </returns>
Response r_hostname()
{
	array<char, 50> buf{};

	gethostname(buf.data(), buf.max_size());
	buf.back() = '\0';

	return Response(buf.data());
}

/// <returns>cpu-name</returns>
Response r_cpu_name()
{
	static array<char, 50> buf{};
	static bool init = false;

	//run only once
	//cpu wont be replaced durning operation
	if (!init) {
		const char* command("cat /proc/cpuinfo | grep 'model name' | head -n 1 | awk -F ':' '{ print $2 }' | sed -e 's/^[[:space:]]*//'");
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
		init = true;
	}

	return Response(buf.data());
}

/// <summary>
/// support function for r_load()
/// </summary>
/// <returns>
/// time data
/// </returns>
bool system_time(uint64_t* idlelast, uint64_t* systemlast, uint64_t* virtuallast)
{
	//source of main inspiration:
	// https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux

	long long unsigned int usertime, nicetime, systemtime, idletime;
	long long unsigned int ioWait, irq, softIrq, steal, guest, guestnice;

	// Iterate over all CPU's and pipe the sum
	const char* cmd{"cat /proc/stat | grep 'cpu' | sed 's/  / /g' | awk -F' ' '{s2+=$2;s3+=$3;s4+=$4;s5+=$5;s6+=$6;s7+=$7;s8+=$8;s9+=$9;s10+=$10;s11+=$11} END {print s2,s3,s4,s5,s6,s7,s8,s9,s10,s11}'"};

	FILE* pipe = popen(cmd, "r");
	if (!pipe) {
		print_error("Bash cmd failed!(cpu)");
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

/// <returns>cpu-name</returns>
Response r_load()
{
	double usagepercent = -1;
	uint64_t idlelast, systemlast, virtuallast;
	uint64_t idlenow, systemnow, virtualnow;

	if (!system_time(&idlelast, &systemlast, &virtuallast)) {
		perror("Shell CMD failed");
		return Response(500);
	}

	using namespace std::this_thread;     // sleep_for, sleep_until
	using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
	using std::chrono::system_clock;
	sleep_for(1s);

	if (!system_time(&idlenow, &systemnow, &virtualnow)) {
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

/// <summary>
/// Client handeling function that calls correct response_handler
/// </summary>
/// <returns>
/// response packet
/// </returns>
Packet& client_handler(Packet& request_p, Response_handler& response_handler)
{
	//what is requested by GET
	string get_req = request_p.get("GET");
	//leaves just /etc and removes HTTP standart
	get_req = get_req.substr(0, get_req.find(" "));

	//gets correct response from response handler
	Response response = response_handler.get(get_req);

	//heading = return code + return code message
	string heading = to_string(response.get_code()) + " " + response.get_e_msg();

	//creates new packet with proper content
	auto* response_p = new Packet();
	response_p->set("HTTP/1.1", heading);
	response_p->set("Content-type", "text/html");
	if (response.get_code() < 500)
		response_p->set("Body", response.get_msg());
	else
		response_p->set("Body", heading);

	return *response_p;
}

int main(int argc, char* argv[])
{
	if (argc == 1) {
		print_error("No port number passed.");
		exit_error("Usage: ./hinfosvc PORT");
	}

	if (argc > 2)
		exit_error("Too many aguments.");

	string portString = argv[1];
	if (!isDigit(portString)) {
		cerr << "\"" << portString << "\" is not valid port number." << endl;
		return EXIT_FAILURE;
	}
	int portNumber = atoi(portString.data());

	Server server;
	server.bind_to(portNumber);

	server.response_handler.set("/hostname", r_hostname);
	server.response_handler.set("/cpu-name", r_cpu_name);
	server.response_handler.set("/load", r_load);

	server.listen_for(5);
	server.start(client_handler);
}