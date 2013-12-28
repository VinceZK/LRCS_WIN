#pragma once
#include "Decoder.h"
#include "StringReader.h"
//#include "ByteReader.h"
#include "Constants.h"
#include "MultiBlock.h"

class __declspec(dllexport) StringDecoder :
	public Decoder
{
public:
	StringDecoder(bool valSorted_);
	StringDecoder(byte* buffer_, bool valSorted_);
	StringDecoder(StringDecoder& decoder_);
	virtual ~StringDecoder();

	virtual void setBuffer(byte* buffer_);
	virtual int getValueSize();
	virtual int getPageLengthBits();


	virtual bool hasNextBlock();
	virtual Block* peekNextBlock();
	virtual Block* getNextBlock();
	virtual Block* getNextBlockSingle();
	virtual bool skipToPos(unsigned int blockPos_);
	virtual bool skipToBlockOnValue(ValPos* rhs_);

	virtual ValPos* getStartVal();
	virtual unsigned int getStartPos();
	virtual ValPos* getEndVal();
	virtual unsigned int getEndPos();

	virtual unsigned int getSize() { return *numStringsPtr; }
	virtual unsigned int getCurrPosition();
protected:
	unsigned int* numStringsPtr;
	unsigned int* startPosPtr;
	unsigned int* ssizePtr;
	StringReader reader;
	MultiBlock* outMultiBlock;
	ValPos* outPair;
	unsigned int currPos;
};

