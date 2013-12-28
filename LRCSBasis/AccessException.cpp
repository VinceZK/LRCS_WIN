#include "AccessException.h"


AccessException::AccessException()
{
}

AccessException::AccessException(char* msg_) : Exception(msg_)
{
}
AccessException::AccessException(const char* msg_) : Exception(msg_)
{
}
AccessException::~AccessException()
{
}
