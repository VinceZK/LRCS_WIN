#pragma once
#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;

class __declspec(dllexport) Log
{
public:
	Log();
	virtual ~Log();
	static int threshold;

	// these need to be called in main (my workaround there not being static constructors 
	// for 
	static void logInit();
	static void logDestroy();

	static bool writeToLog(char* src_, int priority_, char* msg_);
	static bool writeToLog(char* src_, int priority_, string msg_);
	static bool writeToLog(char* src_, int priority_, char* msg_, int var_);

	// Get an output stream on which to print log messages
	// Return pointer may be NULL (in the case log messages from this src
	// and with this priority have been suppressed
	static ostream* getLogStream(char* src_, int priority);

protected:
	static ostream* logStream;
	static bool allowedSrc(char* src_, int priority_);
};

