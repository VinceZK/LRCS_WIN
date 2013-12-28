#pragma once
#include "Exception.h"
class __declspec(dllexport) UnexpectedException : public Exception
{
public:
	UnexpectedException();
	UnexpectedException(char* msg_);
	UnexpectedException(const char* msg_);
	virtual ~UnexpectedException();
};

