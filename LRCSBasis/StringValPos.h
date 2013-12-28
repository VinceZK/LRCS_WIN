#pragma once
#include "ValPos.h"
class __declspec(dllexport) StringValPos :
	public ValPos
{
public:
	StringValPos();
	StringValPos(unsigned short vs_);
	StringValPos(unsigned int pos_, byte* value_, unsigned short vs_);
	StringValPos(StringValPos& valpos_);
	virtual ValPos* clone();
	virtual ~StringValPos();
	virtual void set(byte* value_);
	virtual void set(byte* value_, unsigned short valSize_);
	virtual void set(unsigned int pos_, byte* value_);
	virtual void set(unsigned int pos_, byte* value_, unsigned short valSize_);
	virtual void printVal(std::ostream* o);
	virtual unsigned short getSize();
	unsigned short valsize;
	char* stringvalue;

	//generic function overloads
	virtual bool operator > (ValPos*);
	virtual bool operator < (ValPos*);
	virtual bool operator >= (ValPos*);
	virtual bool operator <= (ValPos*);
	virtual bool operator == (ValPos*);
	virtual bool operator != (ValPos*);
};

