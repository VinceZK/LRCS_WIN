#pragma once
#include "Decoder.h"
#include "IntReader.h"
//#include "ByteReader.h"
#include "MultiBlock.h"
#include "Constants.h"

class __declspec(dllexport) IntDecoder :
	public Decoder
{
public:
	IntDecoder(bool valSorted_);
	IntDecoder(byte* buffer_, bool valSorted_);
	IntDecoder(IntDecoder& decoder_);
	virtual ~IntDecoder();

	virtual void setBuffer(byte* buffer_);
	virtual int getValueSize();
	virtual int getPageLengthBits();

	virtual bool hasNextBlock();
	virtual Block* peekNextBlock();
	virtual Block* getNextBlock();
	virtual Block* getNextBlockSingle();
	virtual bool skipToPos(unsigned int blockLoc_);
	virtual bool skipToBlockOnValue(ValPos* rhs_);

	virtual ValPos* getStartVal();
	virtual unsigned int getStartPos();
	virtual ValPos* getEndVal();
	virtual unsigned int getEndPos();
	virtual unsigned int getSize() { return *numIntsPtr; }
protected:
	unsigned int* numIntsPtr;
	unsigned int* startPosPtr;
	IntReader reader;
	MultiBlock* outBlock;
	ValPos* outPair;
	unsigned int currPos;
};

