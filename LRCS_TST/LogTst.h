#pragma once
#include "UnitTest.h"
class LogTst :
	public UnitTest
{
public:
	LogTst();
	~LogTst();
	bool run(char* arg1, char* arg2);
};

