#include "IntWriter.h"

IntWriter::IntWriter(int bfrSizeInBits_)
{
	buffer = new int[bfrSizeInBits_ / (8 * sizeof(int))];
	currIntPtr = buffer;
	memset(buffer, 0, (bfrSizeInBits_ / (8 * sizeof(int)))); // to prevent rounding errors
	numIntsWritten = 0;
	bufferSizeInInts = bfrSizeInBits_ / (8 * sizeof(int));
	oldBufferSizeInInts = bfrSizeInBits_ / (8 * sizeof(int));
}

IntWriter::~IntWriter()
{

}

bool IntWriter::writeInt(int val_) {
	if (numIntsWritten >= bufferSizeInInts)
		return false;
	*currIntPtr = val_;
	currIntPtr++;
	numIntsWritten++;
	return true;
}

void IntWriter::unWriteInt() {
	//assert(numIntsWritten > 0);
	currIntPtr--;
	numIntsWritten--;
}

bool IntWriter::skipToIntPos(unsigned int pos_) {
	//I shouldn't really cast, but the buffer size shouldn't get that big
	if ((int)pos_ >= bufferSizeInInts) return false;
	currIntPtr = buffer + pos_;
	numIntsWritten = pos_;
	return true;
}
int IntWriter::getNumIntsWritten() {
	return numIntsWritten;
}

int IntWriter::getBufferSize() {
	return 8 * sizeof(int)*bufferSizeInInts;
}

byte* IntWriter::getBuffer() {
	return ((byte*)buffer);
}

void IntWriter::resetBuffer() {
	currIntPtr = buffer;
	numIntsWritten = 0;
}

bool IntWriter::reserveBufferSpace(int numInts) {
	//assert(numInts >= 0);
	bufferSizeInInts -= numInts;
	if (numIntsWritten >= bufferSizeInInts) {
		bufferSizeInInts += numInts;
		return false;
	}
	return true;
}

void IntWriter::unReserveSpace() {
	bufferSizeInInts = oldBufferSizeInInts;
}