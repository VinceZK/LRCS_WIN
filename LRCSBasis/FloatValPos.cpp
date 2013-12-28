#include "FloatValPos.h"

FloatValPos::FloatValPos() {
	type = ValPos::FLOATTYPE;
	position = 0;
	floatvalue = 0;
	value = (byte*)&floatvalue;
}

FloatValPos::FloatValPos(unsigned int pos_, float value_) {
	type = ValPos::FLOATTYPE;
	position = pos_;
	floatvalue = value_;
	value = (byte*)&floatvalue;
}

FloatValPos::FloatValPos(FloatValPos& valpos_) {
	//assert(valpos_.type == ValPos::FLOATTYPE);
	type = valpos_.type;
	position = valpos_.position;
	floatvalue = valpos_.floatvalue;
	value = (byte*)&floatvalue;
}

ValPos* FloatValPos::clone()
{
	ValPos* newVP = new FloatValPos(*this);
	return newVP;
}

FloatValPos::~FloatValPos()
{
	value = NULL;
}

unsigned short FloatValPos::getSize() {
	return sizeof(float);
}

void FloatValPos::set(byte* value_) {
	floatvalue = *(float*)value_;
}

void FloatValPos::set(byte* value_, unsigned short valSize_) {
	floatvalue = *(float*)value_;
}

void FloatValPos::set(unsigned int pos_, byte* value_) {
	position = pos_;
	floatvalue = *(float*)value_;
}

void FloatValPos::set(unsigned int pos_, byte* value_, unsigned short valSize) {
	position = pos_;
	floatvalue = *(float*)value_;
}

void FloatValPos::set(unsigned int type_, unsigned int pos_, byte* value_) {
	//assert(type == ValPos::FLOATTYPE);
	type = type_;
	position = pos_;
	floatvalue = *(float*)value_;
}

void FloatValPos::set(unsigned int type_, unsigned int pos_, byte* value_, unsigned short valSize_) {
	//assert(type == ValPos::FLOATTYPE);
	type = type_;
	position = pos_;
	floatvalue = *(float*)value_;
}

void FloatValPos::printVal(std::ostream* o) {
	(*o) << floatvalue;
}

//generic function overloads
bool FloatValPos::operator> (ValPos* vp) {
	if (vp->type != ValPos::FLOATTYPE)
		throw new UnexpectedException("Incompatible types");
	return floatvalue > *(float*)vp->value;
}

bool FloatValPos::operator>= (ValPos* vp) {
	if (vp->type != ValPos::FLOATTYPE)
		throw new UnexpectedException("Incompatible types");
	return floatvalue >= *(float*)vp->value;
}

bool FloatValPos::operator== (ValPos* vp) {
	if (vp->type != ValPos::FLOATTYPE)
		throw new UnexpectedException("Incompatible types");
	return floatvalue == *(float*)vp->value;
}

bool FloatValPos::operator< (ValPos* vp) {
	if (vp->type != ValPos::FLOATTYPE)
		throw new UnexpectedException("Incompatible types");
	return floatvalue < *(float*)vp->value;
}

bool FloatValPos::operator<= (ValPos* vp) {
	if (vp->type != ValPos::FLOATTYPE)
		throw new UnexpectedException("Incompatible types");
	return floatvalue <= *(float*)vp->value;
}

bool FloatValPos::operator!= (ValPos* vp) {
	if (vp->type != ValPos::FLOATTYPE)
		throw new UnexpectedException("Incompatible types");
	return floatvalue != *(float*)vp->value;
}

//specialized function overloads
bool FloatValPos::operator> (int vp) {
	return floatvalue > vp;
}

bool FloatValPos::operator>= (int vp) {
	return floatvalue >= vp;
}

bool FloatValPos::operator== (int vp) {
	return floatvalue == vp;
}

bool FloatValPos::operator< (int vp) {
	return floatvalue < vp;
}

bool FloatValPos::operator<= (int vp) {
	return floatvalue <= vp;
}

bool FloatValPos::operator!= (int vp) {
	return floatvalue != vp;
}

