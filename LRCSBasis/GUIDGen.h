#pragma once
#include <string>
using namespace std;

class __declspec(dllexport) GUIDGen
{
public:
	GUIDGen();
	~GUIDGen();
	static string getGUID();
};

