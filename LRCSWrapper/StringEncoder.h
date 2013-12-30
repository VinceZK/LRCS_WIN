#pragma once
#include "Encoder.h"
#include "StringWriter.h"
#include "MultiBlock.h"

class __declspec(dllexport) StringEncoder :
	public Encoder
{
public:
	StringEncoder(Operator* dataSrc_, int colIndex_, PosEncoder* posEncoder_, int stringSize, int bfrSizeInBits_);
	virtual ~StringEncoder();
	virtual byte* getPage();
	//virtual byte* getEncodedPosPage(byte** posValue_, unsigned int** posPageSize_);
	virtual void resetPair();
	virtual int getNumValsPerPage(){ return *lengthPtr; }
	virtual short getValSize(){ return (short)*ssizePtr; }
	virtual int getBufferSize();//Get the exact buffer size in the page
	virtual unsigned int getStartPos(){ return (unsigned int)*startPosPtr; }
	virtual unsigned int getPageSize(){ return PAGE_SIZE; }
protected:
	StringWriter* writer;
	byte* buffer;
	int* lengthPtr;
	int* startPosPtr;
	int* ssizePtr;
	MultiBlock* currBlock;
	bool init;
	ValPos* currPair;
};

