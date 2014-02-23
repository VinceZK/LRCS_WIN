#pragma once
#include "ValPos.h"
class __declspec(dllexport) IntValPos :
	public ValPos
{
public:
	IntValPos();
	IntValPos(unsigned int pos_, int value_);
	IntValPos(IntValPos& valpos_);
	virtual ValPos* clone();
	virtual ~IntValPos();
	virtual void set(byte* value_);
	virtual void set(byte* value_, unsigned short valSize_);
	virtual void set(unsigned int pos_, byte* value_);
	virtual void set(unsigned int pos_, byte* value_, unsigned short valSize_);
	virtual void set(eValueType type_, unsigned int pos_, byte* value_);
	virtual void set(eValueType type_, unsigned int pos_, byte* value_, unsigned short valSize_);
	virtual double getDouble() { return (double)intvalue; }
	virtual void printVal(std::ostream* o);
	virtual unsigned short getSize();
	int intvalue;

	//generic function overloads
	virtual bool operator > (ValPos*);
	virtual bool operator < (ValPos*);
	virtual bool operator >= (ValPos*);
	virtual bool operator <= (ValPos*);
	virtual bool operator == (ValPos*);
	virtual bool operator != (ValPos*);

	//specialized function overloads
	virtual bool operator > (int);
	virtual bool operator < (int);
	virtual bool operator >= (int);
	virtual bool operator <= (int);
	virtual bool operator == (int);
	virtual bool operator != (int);
};

