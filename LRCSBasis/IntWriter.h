#pragma once

#ifdef LRCSBASIS_EXPORTS
#define LRCSBasis_API __declspec(dllexport)
#else
#define LRCSBasis_API __declspec(dllimport)
#endif

#include "Constants.h"
#include "Util.h"

class LRCSBasis_API IntWriter
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

