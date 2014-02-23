#include "IntValPos.h"

IntValPos::IntValPos() {
	type = ValPos::INTTYPE;
	position = 0;
	intvalue = 0;
	value = (byte*)&intvalue;
}

IntValPos::IntValPos(unsigned int pos_, int value_) {
	type = ValPos::INTTYPE;
	position = pos_;
	intvalue = value_;
	value = (byte*)&intvalue;
}

IntValPos::IntValPos(IntValPos& valpos_) {
	//assert(valpos_.type == ValPos::INTTYPE);
	type = valpos_.type;
	position = valpos_.position;
	intvalue = valpos_.intvalue;
	value = (byte*)&intvalue;
}

ValPos* IntValPos::clone()
{
	ValPos* newVP = new IntValPos(*this);
	return newVP;
}

IntValPos::~IntValPos()
{
	value = NULL;
}

unsigned short IntValPos::getSize() {
	return sizeof(int);
}

void IntValPos::set(byte* value_) {
	intvalue = *(int*)value_;
}

void IntValPos::set(byte* value_, unsigned short valSize_) {
	intvalue = *(int*)value_;
}

void IntValPos::set(unsigned int pos_, byte* value_) {
	position = pos_;
	intvalue = *(int*)value_;
}

void IntValPos::set(unsigned int pos_, byte* value_, unsigned short valSize) {
	position = pos_;
	intvalue = *(int*)value_;
}

void IntValPos::set(eValueType type_, unsigned int pos_, byte* value_) {
	//assert(type == ValPos::INTTYPE);
	type = type_;
	position = pos_;
	intvalue = *(int*)value_;
}

void IntValPos::set(eValueType type_, unsigned int pos_, byte* value_, unsigned short valSize_) {
	//assert(type == ValPos::INTTYPE);
	type = type_;
	position = pos_;
	intvalue = *(int*)value_;
}

void IntValPos::printVal(std::ostream* o) {
	(*o) << intvalue;
}

//generic function overloads
bool IntValPos::operator> (ValPos* vp) {
	if (vp->type != ValPos::INTTYPE)
		throw new UnexpectedException("Incompatible types");
	return intvalue > *(int*)vp->value;
}

bool IntValPos::operator>= (ValPos* vp) {
	if (vp->type != ValPos::INTTYPE)
		throw new UnexpectedException("Incompatible types");
	return intvalue >= *(int*)vp->value;
}

bool IntValPos::operator== (ValPos* vp) {
	if (vp->type != ValPos::INTTYPE)
		throw new UnexpectedException("Incompatible types");
	return intvalue == *(int*)vp->value;
}

bool IntValPos::operator< (ValPos* vp) {
	if (vp->type != ValPos::INTTYPE)
		throw new UnexpectedException("Incompatible types");
	return intvalue < *(int*)vp->value;
}

bool IntValPos::operator<= (ValPos* vp) {
	if (vp->type != ValPos::INTTYPE)
		throw new UnexpectedException("Incompatible types");
	return intvalue <= *(int*)vp->value;
}

bool IntValPos::operator!= (ValPos* vp) {
	//if (vp->type != ValPos::INTTYPE)
	//  throw new UnexpectedException("Incompatible types");
	return intvalue != *(int*)vp->value;
}

//specialized function overloads
bool IntValPos::operator> (int vp) {
	return intvalue > vp;
}

bool IntValPos::operator>= (int vp) {
	return intvalue >= vp;
}

bool IntValPos::operator== (int vp) {
	return intvalue == vp;
}

bool IntValPos::operator< (int vp) {
	return intvalue < vp;
}

bool IntValPos::operator<= (int vp) {
	return intvalue <= vp;
}

bool IntValPos::operator!= (int vp) {
	return intvalue != vp;
}

