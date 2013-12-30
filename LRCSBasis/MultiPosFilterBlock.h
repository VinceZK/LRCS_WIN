#pragma once

#ifdef LRCSBASIS_EXPORTS
#define LRCSBasis_API __declspec(dllexport)
#else
#define LRCSBasis_API __declspec(dllimport)
#endif

#include "PosFilterBlock.h"
#include <vector>
#include "assert.h"
#include "UnexpectedException.h"
#include "RLEBlock.h"
using namespace std;

class MultiPosFilterCursor;

class LRCSBasis_API MultiPosFilterBlock
{
public:
	MultiPosFilterBlock();
	virtual ~MultiPosFilterBlock();

	virtual void addPosFilterBlock(PosFilterBlock* posFilterBlock_);
	virtual void addPosition(unsigned int pos);
	virtual void addInt(unsigned int int_);
	virtual void optimize();
	virtual void resetBlock();
	virtual MultiPosFilterBlock* clone();

	// Iterator access to blocks
	virtual bool hasNext();
	virtual unsigned int getNext();
	virtual PosFilterBlock* getNextBlock();
	virtual PosFilterBlock* getFirstBlock();
	virtual unsigned int getStartPosition();
	virtual unsigned int getEndPosition();
	virtual PosFilterBlock* getCurrBlock();
	virtual bool setCurrBlock(int i);
	virtual unsigned int getCurrPosition();
	virtual unsigned int getNumBlocks();
	virtual unsigned int getNumValues();
	virtual unsigned int getNumValuesR();
	virtual unsigned int getCurrStartPosition();//Get current start position after filtered a DB page
	virtual void setCurrStartPosition();//Set current start position after filtered a DB page

	virtual bool isNullSet();
	virtual bool isCompleteSet();
	virtual void setCompleteSet(bool flag_);
	virtual void setTriple(RLETriple* triple_);
	virtual void addRLEBlock(RLEBlock* block_);
	MultiPosFilterCursor* getCursor();
	virtual void printBlocks();

protected:
	vector<PosFilterBlock*> posFilterBlockVec;

	unsigned int startPos;
	unsigned int endPos;
	unsigned int currBlockNum;
	unsigned int numValues;
	PosFilterBlock* currBlock;

	bool completeSet;
	bool filterFinished;

	virtual void init();
	virtual bool removePrecedingZero(PosFilterBlock* &posFilterBlock_);

private:
	//bool isBufferSet;
};

