#include "DoubleValPos.h"

DoubleValPos::DoubleValPos() {
	type = ValPos::DOUBLETYPE;
	position = 0;
	doublevalue = 0;
	value = (byte*)&doublevalue;
}

DoubleValPos::DoubleValPos(unsigned int pos_, double value_) {
	type = ValPos::DOUBLETYPE;
	position = pos_;
	doublevalue = value_;
	value = (byte*)&doublevalue;
}

DoubleValPos::DoubleValPos(DoubleValPos& valpos_) {
	//assert(valpos_.type == ValPos::DOUBLETYPE);
	type = valpos_.type;
	position = valpos_.position;
	doublevalue = valpos_.doublevalue;
	value = (byte*)&doublevalue;
}

ValPos* DoubleValPos::clone()
{
	ValPos* newVP = new DoubleValPos(*this);
	return newVP;
}

DoubleValPos::~DoubleValPos()
{
	value = NULL;
}

unsigned short DoubleValPos::getSize() {
	return sizeof(double);
}

void DoubleValPos::set(byte* value_) {
	doublevalue = *(double*)value_;
}

void DoubleValPos::set(byte* value_, unsigned short valSize_) {
	doublevalue = *(double*)value_;
}

void DoubleValPos::set(unsigned int pos_, byte* value_) {
	position = pos_;
	doublevalue = *(double*)value_;
}

void DoubleValPos::set(unsigned int pos_, byte* value_, unsigned short valSize_) {
	position = pos_;
	doublevalue = *(double*)value_;
}

void DoubleValPos::set(unsigned int type_, unsigned int pos_, byte* value_) {
	//assert(type == ValPos::DOUBLETYPE);
	type = type_;
	position = pos_;
	doublevalue = *(double*)value_;
}

void DoubleValPos::set(unsigned int type_, unsigned int pos_, byte* value_, unsigned short valSize_) {
	//assert(type == ValPos::DOUBLETYPE);
	type = type_;
	position = pos_;
	doublevalue = *(double*)value_;
}

void DoubleValPos::printVal(std::ostream* o) {
	(*o) << doublevalue;
}

//generic function overloads
bool DoubleValPos::operator> (ValPos* vp) {
	if (vp->type != ValPos::DOUBLETYPE)
		throw new UnexpectedException("Incompatible types");
	return doublevalue > *(double*)vp->value;
}

bool DoubleValPos::operator>= (ValPos* vp) {
	if (vp->type != ValPos::DOUBLETYPE)
		throw new UnexpectedException("Incompatible types");
	return doublevalue >= *(double*)vp->value;
}

bool DoubleValPos::operator== (ValPos* vp) {
	if (vp->type != ValPos::DOUBLETYPE)
		throw new UnexpectedException("Incompatible types");
	return doublevalue == *(double*)vp->value;
}

bool DoubleValPos::operator< (ValPos* vp) {
	if (vp->type != ValPos::DOUBLETYPE)
		throw new UnexpectedException("Incompatible types");
	return doublevalue < *(double*)vp->value;
}

bool DoubleValPos::operator<= (ValPos* vp) {
	if (vp->type != ValPos::DOUBLETYPE)
		throw new UnexpectedException("Incompatible types");
	return doublevalue <= *(double*)vp->value;
}

bool DoubleValPos::operator!= (ValPos* vp) {
	if (vp->type != ValPos::DOUBLETYPE)
		throw new UnexpectedException("Incompatible types");
	return doublevalue != *(double*)vp->value;
}

//specialized function overloads
bool DoubleValPos::operator> (int vp) {
	return doublevalue > vp;
}

bool DoubleValPos::operator>= (int vp) {
	return doublevalue >= vp;
}

bool DoubleValPos::operator== (int vp) {
	return doublevalue == vp;
}

bool DoubleValPos::operator< (int vp) {
	return doublevalue < vp;
}

bool DoubleValPos::operator<= (int vp) {
	return doublevalue <= vp;
}

bool DoubleValPos::operator!= (int vp) {
	return doublevalue != vp;
}
