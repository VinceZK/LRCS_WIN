#pragma once
#include "Exception.h"
class __declspec(dllexport) CodingException :
	public Exception
{
public:
	CodingException();
	CodingException(char* msg_);
	CodingException(const char* msg_);
	virtual ~CodingException();
};

