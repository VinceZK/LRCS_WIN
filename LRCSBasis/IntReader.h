#pragma once
#include "Constants.h"
#include "Util.h"

class IntReader
{
public:
	IntReader();
	IntReader(byte* buffer_, int numBitsInBuffer_);
	virtual ~IntReader();
	bool readInt(int& value);
	void resetPos();
	bool skipToIntPos(unsigned int intPos_);
	int getCurrentIntPos();
	int getMaxNumInts();
	void setBuffer(byte* buffer_, int numBitsInBuffer_);
	int readFirst();
	int readLast();
	byte* getBuffer(int& numInts);
protected:
	int* buffer;
	int* currPosPtr;
	unsigned int numIntsInBuffer;
	unsigned int currPos;
};

