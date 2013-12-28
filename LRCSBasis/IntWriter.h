#pragma once
#include "Constants.h"
#include "Util.h"

class IntWriter
{
public:
	IntWriter(int bfrSizeInBits_);
	virtual ~IntWriter();

	bool writeInt(int val_);
	void unWriteInt();
	bool skipToIntPos(unsigned int pos_);
	int getNumIntsWritten();
	int getBufferSize();
	byte* getBuffer();
	void resetBuffer();
	bool reserveBufferSpace(int numInts);
	void unReserveSpace();
protected:
	int numIntsWritten;
	int bufferSizeInInts;
	int oldBufferSizeInInts;
	int* buffer;
	int* currIntPtr;
};

