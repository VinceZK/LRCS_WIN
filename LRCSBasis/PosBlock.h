#pragma once
#include "Block.h"
#include <iostream>
#include "UnexpectedException.h"

class PosBlock :
	public Block
{
public:
	PosBlock(ValPos* vp_);
	PosBlock(const PosBlock&);
	Block* clone(Block&);
	virtual ~PosBlock();

	virtual byte* getBuffer(){ return bfrWithHeader; }
	virtual void setBuffer(byte* buffer_);
	virtual void setBufferDirect(byte* buffer_);
	virtual void initBuffer();
	virtual bool addPosition(unsigned int pos);
	virtual void resetBlock();

	virtual bool hasNext(){ return (currPos < *endPos); }
	virtual ValPos* getNext();
	virtual ValPos* peekNext(){ return NULL; }//Not necessary

	virtual ValPos* getPairAtLoc(unsigned int loc_){ return NULL; }//Not necessary
	virtual ValPos* getPairAtLocNotUtility(unsigned int loc_){ return NULL; }//Not necessary
	virtual int getCurrLoc(){ return currPos; }

	virtual int getSize(){ return *numValues; }
	virtual int getSizeInBits(){ return 8 * BLOCK_SIZE; };
	virtual ValPos* getStartPair(){ return NULL; } //Not necessary
	virtual ValPos* getEndPair(){ return NULL; }//Not necessary
	virtual unsigned int getEndPosition(){ return *endPos; }
	virtual unsigned int getStartPosition(){ return *startPos; }
	virtual unsigned int getNumValues(){ return *numValues; };
	virtual void caculateEndInt();
	virtual bool setCurrInt(unsigned int currInt_);

	//virtual void resetBlock();

	// Stream properties
	virtual bool isValueSorted();
	virtual bool isPosSorted();
	// Block properties
	virtual bool isOneValue();
	virtual bool isPosContiguous();
	virtual bool isBlockValueSorted();
	virtual bool isBlockPosSorted();

	virtual void printBlock();
protected:
	byte* bfrWithHeader;
	byte* buffer;
	unsigned int* bufferPtrAsIntArr;
	unsigned int* numValues;
	unsigned int* startPos;
	unsigned int* endPos;
	int* posIndex;
	//unsigned int currStartPos;
	int currIndexInVal;
	unsigned int currInt;
	unsigned int currPos;
	unsigned int endInt;

	virtual void setPosIndex(int v, int* pidx, int& currIndexInVal);
	virtual void init();
	virtual void printBits(int bits);

private:
	bool isBufferSet;
	ValPos* vp;
};

