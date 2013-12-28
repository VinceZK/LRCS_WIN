#include "CodingException.h"

CodingException::CodingException()
{
}

CodingException::CodingException(char* msg_) : Exception(msg_)
{
}

CodingException::CodingException(const char* msg_) : Exception(msg_)
{
}

CodingException::~CodingException()
{
}
