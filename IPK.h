#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <cstring>
#include <map>
#include <vector>

#include <math.h>
#include <unistd.h>
#include <limits.h>

#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <array>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>

using namespace std;

//check if string is whole positive number
int isDigit(string s) {
	if (s.size() == 0)
		return 0;
	for (char& c : s)
	{
		if (!isdigit(c))
			return 0;
	}
	return 1;
}

void handle_error(string msg){
	perror(msg.c_str());
	exit(EXIT_FAILURE);
}
