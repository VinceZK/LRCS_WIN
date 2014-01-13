#include "LongValPos.h"

LongValPos::LongValPos() {
	type = ValPos::LONGTYPE;
	longvalue = 0;
	position = 0;
	value = (byte*)&longvalue;
}

LongValPos::LongValPos(unsigned int pos_, long long value_) {
	type = ValPos::LONGTYPE;
	position = pos_;
	longvalue = value_;
	value = (byte*)&longvalue;
}

LongValPos::LongValPos(LongValPos& valpos_) {
	//assert(valpos_.type == ValPos::LONGTYPE);
	type = valpos_.type;
	position = valpos_.position;
	longvalue = valpos_.longvalue;
	value = (byte*)&longvalue;
}

ValPos* LongValPos::clone()
{
	ValPos* newVP = new LongValPos(*this);
	return newVP;
}

LongValPos::~LongValPos()
{
	value = NULL;
}

unsigned short LongValPos::getSize() {
	return sizeof(long long);
}

void LongValPos::set(byte* value_) {
	longvalue = *(long long*)value_;
}

void LongValPos::set(byte* value_, unsigned short valSize_) {
	longvalue = *(long long*)value_;
}

void LongValPos::set(unsigned int pos_, byte* value_) {
	position = pos_;
	longvalue = *(long long*)value_;
}

void LongValPos::set(unsigned int pos_, byte* value_, unsigned short valSize_) {
	position = pos_;
	longvalue = *(long long*)value_;
}

void LongValPos::set(eValueType type_, unsigned int pos_, byte* value_) {
	//assert(type == ValPos::LONGTYPE);
	type = type_;
	position = pos_;
	longvalue = *(long long*)value_;
}

void LongValPos::set(eValueType type_, unsigned int pos_, byte* value_, unsigned short valSize_) {
	//assert(type == ValPos::LONGTYPE);
	type = type_;
	position = pos_;
	longvalue = *(long long*)value_;
}


void LongValPos::printVal(std::ostream* o) {
	(*o) << longvalue;
}

//generic function overloads
bool LongValPos::operator> (ValPos* vp) {
	if (vp->type != ValPos::LONGTYPE)
		throw new UnexpectedException("Incompatible types");
	return longvalue > *(long long*)vp->value;
}

bool LongValPos::operator>= (ValPos* vp) {
	if (vp->type != ValPos::LONGTYPE)
		throw new UnexpectedException("Incompatible types");
	return longvalue >= *(long long*)vp->value;
}

bool LongValPos::operator== (ValPos* vp) {
	if (vp->type != ValPos::LONGTYPE)
		throw new UnexpectedException("Incompatible types");
	return longvalue == *(long long*)vp->value;
}

bool LongValPos::operator< (ValPos* vp) {
	if (vp->type != ValPos::LONGTYPE)
		throw new UnexpectedException("Incompatible types");
	return longvalue < *(long long*)vp->value;
}

bool LongValPos::operator<= (ValPos* vp) {
	if (vp->type != ValPos::LONGTYPE)
		throw new UnexpectedException("Incompatible types");
	return longvalue <= *(long long*)vp->value;
}

bool LongValPos::operator!= (ValPos* vp) {
	if (vp->type != ValPos::LONGTYPE)
		throw new UnexpectedException("Incompatible types");
	return longvalue != *(long long*)vp->value;
}

//specialized function overloads
bool LongValPos::operator> (int vp) {
	return longvalue > vp;
}

bool LongValPos::operator>= (int vp) {
	return longvalue >= vp;
}

bool LongValPos::operator== (int vp) {
	return longvalue == vp;
}

bool LongValPos::operator< (int vp) {
	return longvalue < vp;
}

bool LongValPos::operator<= (int vp) {
	return longvalue <= vp;
}

bool LongValPos::operator!= (int vp) {
	return longvalue != vp;
}
