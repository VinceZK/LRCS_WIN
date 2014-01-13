#pragma once
#include "ValPos.h"
class __declspec(dllexport) FloatValPos :
	public ValPos
{
public:
	FloatValPos();
	FloatValPos(unsigned int pos_, float value_);
	FloatValPos(FloatValPos& valpos_);
	virtual ValPos* clone();
	virtual ~FloatValPos();
	virtual void set(byte* value_);
	virtual void set(byte* value_, unsigned short valSize_);
	virtual void set(unsigned int pos_, byte* value_);
	virtual void set(unsigned int pos_, byte* value_, unsigned short valSize_);
	virtual void set(eValueType type_, unsigned int pos_, byte* value_);
	virtual void set(eValueType type_, unsigned int pos_, byte* value_, unsigned short valSize_);
	virtual double getDouble() { return (double)floatvalue; }
	virtual void printVal(std::ostream* o);
	virtual unsigned short getSize();
	float floatvalue;

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

