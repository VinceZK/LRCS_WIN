#pragma once
#include "Decoder.h"
#include "IntReader.h"
#include "IntWriter.h"
//#include "ByteReader.h"
#include "MultiBlock.h"
#include "Constants.h"
//#include <db_cxx.h>

class __declspec(dllexport) PosDecoder :
	public Decoder
{
public:
	PosDecoder(bool valSorted_);
	PosDecoder(byte* buffer_, bool valSorted_);
	PosDecoder(PosDecoder& decoder_);
	virtual ~PosDecoder();

	virtual void setBuffer(byte* buffer_);
	virtual int getValueSize();
	virtual int getPageLengthBits();

	virtual bool hasNextBlock();
	virtual Block* peekNextBlock();
	virtual Block* getNextBlock();
	virtual Block* getNextBlockSingle();
	virtual bool skipToPos(unsigned int blockLoc_);
	virtual bool skipToBlockOnValue(ValPos* rhs_);
	//virtual bool skipToBlockOnValue(int value_);



	virtual ValPos* getStartVal();
	virtual unsigned int getStartPos();
	virtual ValPos* getEndVal();
	virtual unsigned int getEndPos();

	virtual unsigned int getSize() { return *numIntsPtr; }
protected:
	unsigned int* numIntsPtr;
	unsigned int startPos;
	IntReader reader;
	MultiBlock* outBlock;
	ValPos* outPair;
	unsigned int currPos;
};

