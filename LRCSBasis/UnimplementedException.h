#pragma once
#include "Exception.h"
class __declspec(dllexport) UnimplementedException :
	public Exception
{
public:
	UnimplementedException();
	UnimplementedException(char* msg_);
	UnimplementedException(const char* msg_);
	virtual ~UnimplementedException();
};

