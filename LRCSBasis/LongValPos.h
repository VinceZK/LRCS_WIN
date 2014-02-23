#pragma once
#include "ValPos.h"
class __declspec(dllexport) LongValPos :
	public ValPos
{
public:
	LongValPos();
	LongValPos(unsigned int pos_, long long value_);
	LongValPos(LongValPos& valpos_);
	virtual ValPos* clone();
	virtual ~LongValPos();
	virtual void set(byte* value_);
	virtual void set(byte* value_, unsigned short valSize_);
	virtual void set(unsigned int pos_, byte* value_);
	virtual void set(unsigned int pos_, byte* value_, unsigned short valSize_);
	virtual void set(eValueType, unsigned int pos_, byte* value_);
	virtual void set(eValueType, unsigned int pos_, byte* value_, unsigned short valSize_);
	virtual double getDouble() { return (double)longvalue; }
	virtual void printVal(std::ostream* o);
	virtual unsigned short getSize();
	long long longvalue;

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

