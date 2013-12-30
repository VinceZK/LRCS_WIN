#pragma once

#ifdef LRCSBASIS_EXPORTS
#define LRCSBasis_API __declspec(dllexport)
#else
#define LRCSBasis_API __declspec(dllimport)
#endif
//Abstract Class to define a cursor
#include "Constants.h"

class LRCSBasis_API Cursor
{
public:
	Cursor();

	virtual ~Cursor() = 0;

	virtual	unsigned int getNext() = 0;

	virtual void setCurrStartPosition() = 0;

	virtual unsigned int getCurrStartPosition() = 0;

	virtual unsigned int getCurrPosition() = 0;

	virtual void resetCursor() = 0;


protected:
};

