#pragma once
#include "UnexpectedException.h"
#include "UnimplementedException.h"
#include "Constants.h"
#include <string.h>
#include <iostream>

class __declspec(dllexport) ValPos
{
public:
	ValPos() : type(NOTYPE), position(0), value(NULL) {}

	virtual ValPos* clone() = 0;
	virtual ~ValPos();
	virtual void set(byte* value_) = 0;
	virtual void set(byte* value_, unsigned short valSize_) = 0;
	virtual void set(unsigned int pos_, byte* value_) = 0;
	virtual void set(unsigned int pos_, byte* value_, unsigned short valSize_) = 0;
	virtual void printVal(std::ostream* o) = 0;
	virtual unsigned short getSize() = 0;
	unsigned int type;
	unsigned int position;
	byte* value;

	enum {
		NOTYPE = 0,
		INTTYPE = 1,
		FLOATTYPE = 2,
		STRINGTYPE = 3,
		LONGTYPE = 4,
		DOUBLETYPE = 5
	};

	//generic function overloads
	virtual bool operator > (ValPos*) = 0;
	virtual bool operator < (ValPos*) = 0;
	virtual bool operator >= (ValPos*) = 0;
	virtual bool operator <= (ValPos*) = 0;
	virtual bool operator == (ValPos*) = 0;
	virtual bool operator != (ValPos*) = 0;
};

