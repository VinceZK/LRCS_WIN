#include "stdafx.h"
#include "SysTime.h"


SysTime::SysTime()
{
}


SysTime::~SysTime()
{
}

string SysTime::getCurrTime(){
	time_t t;  //seconds
	tm* local; //local time 
	tm* gmt;   //greenwidth time
	char buf[15] = { 0 };

	t = time(NULL); 
	local = localtime(&t); 
	local->tm_year = local->tm_year + 1900;
	local->tm_mon = local->tm_mon + 1;

	_snprintf(buf, 14, "%04d%02d%02d%02d%02d%02d", local->tm_year, local->tm_mon,
		local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec);
	return string(buf);
}