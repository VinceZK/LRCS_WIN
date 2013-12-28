#include "UnimplementedException.h"


UnimplementedException::UnimplementedException()
{
}

UnimplementedException::UnimplementedException(char* msg_) : Exception(msg_)
{
}
UnimplementedException::UnimplementedException(const char* msg_) : Exception(msg_)
{
}
UnimplementedException::~UnimplementedException()
{
}
