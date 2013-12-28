#pragma once
#include "Block.h"
#include "BitReader.h"

class BitBlock :
	public Block
{
public:
	BitBlock(bool valSorted_);
	BitBlock(BitBlock& block_);
	virtual ~BitBlock();
	Block* clone(Block&);

	virtual byte* getBuffer();
	virtual void setBuffer(byte* bfr_, int bfrSize_);
	virtual bool hasNext();
	virtual bool hasNext(int value_);
	virtual ValPos* getNext();
	virtual ValPos* peekNext();

	// Zero indexed, gets the pair at this loc
	virtual ValPos* getPairAtLoc(unsigned int loc_);

	//Like getPairAtLoc except set the regular pair variable rather than 
	//the utilityPair variable so can be called by getNext(). 
	virtual ValPos* getPairAtLocNotUtility(unsigned int loc_);
	virtual int getCurrLoc();

	// return size of block	
	virtual unsigned int getFieldSize();
	virtual int getSize();
	virtual int getSizeInBits();
	virtual ValPos* getStartPair();
	virtual ValPos* getEndPair();
	virtual unsigned int getEndPosition();

	virtual void resetBlock();

	// Stream properties
	virtual bool isValueSorted();
	virtual bool isPosSorted();

	// Block properties
	virtual bool isOneValue();
	virtual bool isPosContiguous();
	virtual bool isBlockValueSorted();
	virtual bool isBlockPosSorted();
protected:
	BitReader* reader;
	ValPos* outPair;
	byte* bfr;
	unsigned int bfrSize;
	unsigned int currLoc;

	bool init;
	unsigned int fieldSize;
	unsigned int startPos;
	unsigned int numValues;
	bool valSorted;
};

