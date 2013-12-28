#include "Exception.h"
#include <string>

Exception::Exception()
{
	msg = NULL;
}

Exception::Exception(const char* msg_)
{
	msg = new char[strlen(msg_) + 1];
	strcpy(msg, msg_);
}

Exception::Exception(char* msg_)
{
	msg = msg_;
}

Exception::~Exception()
{
	if (msg != NULL)
		delete[] msg;
}
