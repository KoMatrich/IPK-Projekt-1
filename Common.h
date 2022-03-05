#pragma once

using namespace std;

#include <string>
#include <iostream>
#include <cstdarg>

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

/// <summary>
/// prints msg and errno (stderr), then exits with failure
/// </summary>
void inline exit_errno(string msg){
	perror(msg.c_str());
	exit(EXIT_FAILURE);
}

/// <summary>
/// prints msg (stderr), then exits with failure
/// </summary>
void inline exit_error(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
	va_end(args);
	exit(EXIT_FAILURE);
}

/// <summary>
/// prints msg (stderr)
/// </summary>
void inline print_error(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
	va_end(args);
}