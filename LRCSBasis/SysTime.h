#pragma once
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <string>

using namespace std;
class __declspec(dllexport) SysTime
{
public:
	SysTime();
	~SysTime();
	static string getCurrTime();
};

