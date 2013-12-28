#pragma once
#include "Cursor.h"
#include "MultiPosFilterBlock.h"
#include "PosFilterCursor.h"

class  MultiPosFilterCursor :
	public Cursor
{
public:
	MultiPosFilterCursor(MultiPosFilterBlock* MPFB_);

	// destructor
	~MultiPosFilterCursor();

	unsigned int getNext();

	bool hasNext();

	void setCurrStartPosition();

	unsigned int getCurrStartPosition();

	unsigned int getCurrPosition();

	void resetCursor();

	bool isFilterFinished();

protected:

	vector<PosFilterBlock*> posFilterBlockVec;

	unsigned int startPos;
	unsigned int endPos;
	unsigned int currBlockNum;
	//unsigned int numValues;
	PosFilterBlock* currBlock;
	Cursor* currSPFC;
	bool completeSet;
	bool filterFinished;

	bool setCurrBlock(int i);
};

