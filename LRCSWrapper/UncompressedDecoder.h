#pragma once
#include "Decoder.h"
#include "StringReader.h"
//#include "ByteReader.h"
#include "MultiBlock.h"
#include "Constants.h"

class __declspec(dllexport) UncompressedDecoder :
	public Decoder
{
public:
	UncompressedDecoder(bool valSorted_, unsigned short type_);
	UncompressedDecoder(byte* buffer_, bool valSorted_, unsigned short type_);
	UncompressedDecoder(UncompressedDecoder& decoder_);
	virtual ~UncompressedDecoder();

	virtual void setBuffer(byte* buffer_);
	virtual int getValueSize();
	virtual int getPageLengthBits();

	virtual bool hasNextBlock();
	virtual Block* peekNextBlock();
	virtual Block* getNextBlock();
	virtual Block* getNextBlockSingle();
	virtual bool skipToPos(unsigned int blockLoc_);

	virtual bool skipToBlockOnValue(int value_);
	virtual bool skipToBlockOnValue(ValPos* rhs_){ throw new UnimplementedException(""); }



	virtual ValPos* getStartVal();
	virtual unsigned int getStartPos();
	virtual ValPos* getEndVal();
	virtual unsigned int getEndPos();

	virtual unsigned int getSize() { return *numValsPtr; }
protected:
	unsigned int* numValsPtr;
	unsigned int* startPosPtr;
	unsigned int* ssizePtr;
	StringReader reader;
	MultiBlock* outBlock;
	ValPos* outPair;
	unsigned int currPos;
	unsigned short type;
	bool valSorted;
};

