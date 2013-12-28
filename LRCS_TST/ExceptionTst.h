#pragma once
#include "UnitTest.h"
class ExceptionTst :
	public UnitTest
{
public:
	ExceptionTst();
	virtual ~ExceptionTst();
	bool run(char* arg1, char* arg2);

private:
	void throwAccessException();
	void throwCodingException();
	void throwUnexpectedException();
	void throwUnimplementedException();
};

