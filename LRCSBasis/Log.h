#pragma once
#include <iostream>
#include <cstring>
#include <fstream>
#include <windows.h>
#include <map>

#define LOG_SLOT "../SystemDB/LOG_SLOT"
#define SEPARATOR '\t'
#define MAXLINSIZE 1024

using namespace std;

class __declspec(dllexport) Log
{
public:
	Log();
	virtual ~Log();

	// these need to be called in main (my workaround there not being static constructors 
	// for 
	static void logInit();
	static void logDestroy();

	static bool writeToLog(char* src_, int priority_, char* msg_, ...);
	static bool writeToLog(char* src_, int priority_, string msg_);
	static bool writeToLog(char* src_, int priority_, char* msg_, int var_);

protected:
	static CRITICAL_SECTION csLock;
	static void initLogSlot();
	static int getCurrLogIndex();
	static void setCurrLogIndex(int slot_);
	static bool openNewLogFile();
	static void writeLogStream(char* src_, char* msg_);

	static map<int, string>* logSlot;
	static ostream* logStream;
	static string logFileName;
	static bool log_2_file;
	static int threshold;
	static int logSize;
	static bool no_log;
	static char* logstr;
};

