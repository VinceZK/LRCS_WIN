#pragma once
#include "Constants.h"
#include "Util.h"

class StringWriter
{
public:
	StringWriter(int stringSize_, int bfrSizeInBits_);
	virtual ~StringWriter();

	bool writeString(char* val_);
	bool writeString(char* val_, int numBytes);
	bool skipToIntPos(unsigned int pos_);
	bool skipToBytePos(unsigned int pos_);
	int getNumBytesWritten();
	int getNumValuesWritten();
	int getBufferSize();
	int getStringSize();
	byte* getBuffer();
	void resetBuffer();
	bool reserveBufferSpace(int numInts);
	void unReserveSpace();
	void unWriteString();

protected:
	unsigned int numBytesWritten;
	unsigned int bufferSizeInBytes;
	unsigned int oldBufferSizeInBytes;
	unsigned int stringSize;
	byte* buffer;
	byte* currBytePtr;
};

