#pragma once
#include "ValPos.h"
class __declspec(dllexport) DoubleValPos :
	public ValPos
{
public:
	DoubleValPos();
	DoubleValPos(unsigned int pos_, double value_);
	DoubleValPos(DoubleValPos& valpos_);
	virtual ValPos* clone();
	virtual ~DoubleValPos();
	virtual void set(byte* value_);
	virtual void set(byte* value_, unsigned short valSize_);
	virtual void set(unsigned int pos_, byte* value_);
	virtual void set(unsigned int pos_, byte* value_, unsigned short valSize_);
	virtual void set(eValueType type_, unsigned int pos_, byte* value_);
	virtual void set(eValueType type_, unsigned int pos_, byte* value_, unsigned short valSize_);
	virtual double getDouble() { return (double)doublevalue; }
	virtual void printVal(std::ostream* o);
	virtual unsigned short getSize();
	double doublevalue;

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

