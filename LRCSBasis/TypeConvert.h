#pragma once
#include <string>

using namespace std;
class TypeConvert
{
public:
	TypeConvert();
	~TypeConvert();
	static bool StringToBoolean(string& value_);
	static int StringToInt(string& value_);
};

