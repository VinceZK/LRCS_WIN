#pragma once
#include "ValPos.h"
class __declspec(dllexport) NullValPos :
	public ValPos
{
public:
	NullValPos();
	NullValPos(unsigned int pos_);
	NullValPos(NullValPos& valpos_);
	virtual ValPos* clone();
	virtual ~NullValPos();
	virtual void set(byte* value_);
	virtual void set(byte* value_, unsigned short valSize_);
	virtual void set(unsigned int pos_, byte* value_);
	virtual void set(unsigned int pos_, byte* value_, unsigned short valSize_);
	virtual void set(eValueType type_, unsigned int pos_, byte* value_);
	virtual void set(eValueType type_, unsigned int pos_, byte* value_, unsigned short valSize_);
	virtual void printVal(std::ostream* o);
	virtual unsigned short getSize();

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

