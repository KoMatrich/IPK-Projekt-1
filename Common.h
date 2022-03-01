#pragma once

using namespace std;

#include <string>
#include <iostream>

//check if string is whole positive number
int inline isDigit(string s) {
	if (s.size() == 0)
		return 0;
	for (char& c : s)
	{
		if (!isdigit(c))
			return 0;
	}
	return 1;
}

void inline handle_error(string msg){
	perror(msg.c_str());
	exit(EXIT_FAILURE);
}
