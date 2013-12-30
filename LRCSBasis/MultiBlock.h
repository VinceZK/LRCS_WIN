#pragma once

#ifdef LRCSBASIS_EXPORTS
#define LRCSBasis_API __declspec(dllexport)
#else
#define LRCSBasis_API __declspec(dllimport)
#endif

#include "Block.h"
#include "CodingException.h"
#include <iostream>
#include "MultiPosFilterCursor.h"

class LRCSBasis_API MultiBlock :
	public Block
{
public:
	MultiBlock(bool valSorted_, bool posContig_, bool posSorted_);
	MultiBlock(bool valSorted_, bool posContig_, bool posSorted_, short type_);
	MultiBlock(bool valSorted_, bool posContig_, bool posSorted_, short type_, short valsize_);
	MultiBlock(bool valSorted_, bool posContig_, bool posSorted_, ValPos* vp_);
	MultiBlock(const MultiBlock&);
	virtual ~MultiBlock();
	Block* clone(Block&);
	virtual void init(bool valSorted_, bool posContig_, bool posSorted_);
	virtual void setBuffer(int startpos_, int numValues_, short valsize_, byte* buffer_);
	void setBufferDirect(int startpos_, int numValues_, short valsize_, byte* buffer_);
	virtual void resetBlock();

	// Iterator access to block
	virtual bool hasNext();
	virtual ValPos* getNext();
	virtual ValPos* peekNext();
	virtual unsigned int getPosition();
	virtual unsigned int getLastPosition();
	virtual unsigned int getEndPosition();
	virtual void cutOffEnd(int endPos);
	virtual byte* getBuffer();
	MultiBlock* copySubset(int fromPos, int untilPos);
	virtual void filterWithPos(MultiPosFilterCursor* filterCursor);

	// Zero indexed, gets the pair at this pos_
	virtual ValPos* getPairAtLoc(unsigned int loc_);

	//Like getPairAtLoc except set the regular pair variable rather than 
	//the utilityPair variable so can be called by getNext(). 
	virtual ValPos* getPairAtLocNotUtility(unsigned int loc_);
	virtual int getCurrLoc();
	//virtual void setValAtLoc(unsigned int loc_, int val);
	virtual bool skipToLoc(unsigned int loc_);

	// return size of block (number of values)
	virtual int getSize();
	//return buffer size in block
	virtual int getSizeInBits();
	virtual ValPos* getStartPair();

	// Stream properties
	virtual bool isValueSorted();
	virtual bool isPosSorted();

	// Block properties
	virtual bool isOneValue();
	virtual bool isPosContiguous();
	virtual bool isBlockValueSorted();
	virtual bool isBlockPosSorted();

	virtual bool isSplittable();
	Block* split(int npositions);

	int currPos;

protected:
	bool valSorted;
	bool posContig;
	bool posSorted;

	bool bufferdirect;
	byte* buffer;
	int buffer_size;
	unsigned int numValues;
	int startPos;
	byte* currPosPtr;
};

