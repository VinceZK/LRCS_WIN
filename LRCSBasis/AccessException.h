#pragma once
#include "Exception.h"
class __declspec(dllexport) AccessException :
	public Exception
{
public:
	AccessException();
	AccessException(char* msg_);
	AccessException(const char* msg_);
	virtual ~AccessException();
};

