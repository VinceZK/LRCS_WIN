#include "IntReader.h"

IntReader::IntReader() {
	buffer = NULL;
	currPosPtr = NULL;
	numIntsInBuffer = 0;
	currPos = 0;
}
IntReader::IntReader(byte* buffer_, int numBitsInBuffer_)
{
	buffer = (int*)buffer_;
	currPosPtr = buffer;
	numIntsInBuffer = numBitsInBuffer_ / (8 * sizeof(int));
	currPos = 0;
}

IntReader::~IntReader()
{

}

byte* IntReader::getBuffer(int& numInts) {
	if (currPos >= numIntsInBuffer) return NULL;
	numInts = numIntsInBuffer - currPos;
	currPos = numIntsInBuffer;
	return ((byte*)currPosPtr);
}

bool IntReader::readInt(int& value) {
	if (currPos >= numIntsInBuffer) return false;
	value = *currPosPtr;
	currPosPtr++;
	currPos++;
	return true;
}
void IntReader::resetPos() {
	currPosPtr = buffer;
	currPos = 0;
}
bool IntReader::skipToIntPos(unsigned int intPos_) {
	if (intPos_ >= numIntsInBuffer) return false;
	currPos = intPos_;
	currPosPtr = buffer + intPos_;
	return true;
}
int IntReader::readFirst() {
	return buffer[0];
}
int IntReader::readLast() {
	return buffer[numIntsInBuffer - 1];
}

int IntReader::getCurrentIntPos() {
	return currPos;
}
int IntReader::getMaxNumInts() {
	return numIntsInBuffer;
}
void IntReader::setBuffer(byte* buffer_, int numBitsInBuffer_) {
	buffer = (int*)buffer_;
	currPosPtr = buffer;
	numIntsInBuffer = numBitsInBuffer_ / (8 * sizeof(int));
	currPos = 0;
}