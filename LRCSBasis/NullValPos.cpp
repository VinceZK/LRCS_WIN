#include "NullValPos.h"

NullValPos::NullValPos() {
	type = ValPos::NOTYPE;
	position = 0;
	value = NULL;
}

NullValPos::NullValPos(unsigned int pos_) {
	type = ValPos::NOTYPE;
	position = pos_;
	value = NULL;
}

NullValPos::NullValPos(NullValPos& valpos_) {
	//assert(valpos_.type == ValPos::NOTYPE);
	type = valpos_.type;
	position = valpos_.position;
	value = NULL;
}

ValPos* NullValPos::clone()
{
	ValPos* newVP = new NullValPos(*this);
	return newVP;
}

NullValPos::~NullValPos()
{
	value = NULL;
}

unsigned short NullValPos::getSize() {
	return 0;
}

void NullValPos::set(byte* value_) {
	//assert(value_ == NULL);
}

void NullValPos::set(byte* value_, unsigned short valSize_) {
	//assert(value_ == NULL);
}

void NullValPos::set(unsigned int pos_, byte* value_) {
	//assert(value_ == NULL);
	position = pos_;
}

void NullValPos::set(unsigned int pos_, byte* value_, unsigned short valSize_) {
	//assert(value_ == NULL);
	position = pos_;
}

void NullValPos::set(unsigned int type_, unsigned int pos_, byte* value_) {
	//assert(type == ValPos::NOTYPE);
	//assert(value_ == NULL);
	type = type_;
	position = pos_;
}

void NullValPos::set(unsigned int type_, unsigned int pos_, byte* value_, unsigned short valSize_) {
	//assert(type == ValPos::NOTYPE);
	//assert(value_ == NULL);
	type = type_;
	position = pos_;
}

void NullValPos::printVal(std::ostream* o) {
	throw new UnexpectedException("Trying to print a Null value ...");
}


//generic function overloads
bool NullValPos::operator> (ValPos* vp) {
	return false;
}

bool NullValPos::operator>= (ValPos* vp) {
	return false;
}

bool NullValPos::operator== (ValPos* vp) {
	return false;
}

bool NullValPos::operator< (ValPos* vp) {
	return false;
}

bool NullValPos::operator<= (ValPos* vp) {
	return false;
}

bool NullValPos::operator!= (ValPos* vp) {
	return false;
}

//specialized function overloads
bool NullValPos::operator> (int vp) {
	return false;
}

bool NullValPos::operator>= (int vp) {
	return false;
}

bool NullValPos::operator== (int vp) {
	return false;
}

bool NullValPos::operator< (int vp) {
	return false;
}

bool NullValPos::operator<= (int vp) {
	return false;
}

bool NullValPos::operator!= (int vp) {
	return false;
}