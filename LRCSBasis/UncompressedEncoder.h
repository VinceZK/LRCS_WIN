#pragma once
#include "Encoder.h"
#include "StringWriter.h"
#include "Block.h"
class __declspec(dllexport) UncompressedEncoder :
	public Encoder
{
public:
	UncompressedEncoder(Operator* dataSrc_, int colIndex_, int stringSize, int bfrSizeInBits_);
	virtual ~UncompressedEncoder();
	virtual byte* getPage();
	virtual void resetPair();
	virtual bool writeVal(char* val_, unsigned int pos_);
	virtual int getNumValsPerPage(){ return *lengthPtr; }
	virtual short getValSize(){ return (short)*ssizePtr; }
	virtual int getBufferSize();
	virtual unsigned int getStartPos(){ return (unsigned int)*startPosPtr; }
	virtual unsigned int getPageSize(){ return PAGE_SIZE; }
protected:
	//int totalNum;
	//int totalLength;
	StringWriter* writer;
	byte* buffer;
	int* lengthPtr;
	int* startPosPtr;
	int* ssizePtr;
	//int ssize;
	Block* currBlock;
	bool init;
	ValPos* currVP;
};

