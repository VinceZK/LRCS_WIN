#include "UnexpectedException.h"


UnexpectedException::UnexpectedException()
{
}
UnexpectedException::UnexpectedException(char* msg_) : Exception(msg_)
{
}
UnexpectedException::UnexpectedException(const char* msg_) : Exception(msg_)
{
}
UnexpectedException::~UnexpectedException()
{
}
