#pragma once

#ifdef LRCSBASIS_EXPORTS
#define LRCSBasis_API __declspec(dllexport)
#else
#define LRCSBasis_API __declspec(dllimport)
#endif

#include "Constants.h"
#include "Util.h"

class LRCSBasis_API StringWriter
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

