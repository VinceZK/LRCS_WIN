#pragma once
class __declspec(dllexport) Exception
{
public:
	Exception();
	Exception(const char *msg_);
	Exception(char *msg_);
	virtual ~Exception();
	char* msg;
};

