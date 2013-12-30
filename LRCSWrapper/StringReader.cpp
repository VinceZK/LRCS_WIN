#include "StringReader.h"

StringReader::StringReader() {
	buffer = NULL;
	currPosPtr = NULL;
	numBytesInBuffer = 0;
	currPos = 1;
}

StringReader::~StringReader()
{

}

void StringReader::setStringSize(int stringSize_) {
	stringSize = stringSize_;
}

byte* StringReader::getBuffer(int& numVals) {
	if ((currPos*stringSize)>numBytesInBuffer)return NULL;
	numVals = (numBytesInBuffer / stringSize) - currPos + 1;
	return (currPosPtr);
}

bool StringReader::readString(char* value_) {
	if ((currPos*stringSize)>numBytesInBuffer) return false;
	memcpy(value_, currPosPtr, stringSize);
	currPosPtr += stringSize;
	currPos++;
	return true;
}

void StringReader::unReadString() {
	////assert(currPos>0);
	currPosPtr -= stringSize;
	currPos--;
}

bool StringReader::hasNextString() {
	return ((currPos*stringSize) <= (numBytesInBuffer - stringSize));
}
void StringReader::resetPos() {
	currPosPtr = buffer;
	currPos = 1;
}
bool StringReader::skipToStringPos(unsigned int intPos_) {
	if ((intPos_*stringSize)>numBytesInBuffer) return false;
	currPos = intPos_;
	currPosPtr = buffer + ((intPos_ - 1)*stringSize);
	return true;
}

int StringReader::getCurrentPos() {
	return currPos;
}

void StringReader::setBuffer(byte* buffer_, int numBitsInBuffer_) {
	buffer = buffer_;
	currPosPtr = buffer;
	numBytesInBuffer = numBitsInBuffer_ / 8;
	currPos = 1;
}

StringValPos* StringReader::readFirst() {
	StringValPos* vp = new StringValPos(stringSize);
	vp->set(1, buffer);
	return vp;
}
StringValPos* StringReader::readLast() {
	byte* lastValPtr;
	int lastPos = numBytesInBuffer / stringSize;
	StringValPos* vp = new StringValPos(stringSize);
	lastValPtr = buffer + numBytesInBuffer - stringSize;
	vp->set(lastPos, lastValPtr);
	return vp;
}
bool StringReader::skipToValPos(ValPos* vp_){
	return biSearch(vp_, 1, numBytesInBuffer / stringSize);
}

bool StringReader::biSearch(ValPos* vp_, unsigned int begin, unsigned int last) {
	if (begin > last)return false;
	unsigned int mid = (begin + last) / 2;

	skipToStringPos(mid);
	if (memcmp(vp_->value, currPosPtr, stringSize) == 0){
		vp_->position = currPos;
		return true;
	}

	if (memcmp(vp_->value, currPosPtr, stringSize) < 0){
		biSearch(vp_, begin, mid - 1);
	}
	else{
		biSearch(vp_, mid + 1, last);
	}
}