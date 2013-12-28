#pragma once
#include "Cursor.h"

class PosFilterBlock;

class  PosFilterCursor :
	public Cursor
{
public:
	PosFilterCursor(PosFilterBlock* SPFB_);

	// destructor
	~PosFilterCursor();

	unsigned int getNext();

	bool hasNext();

	void setCurrStartPosition();

	unsigned int getCurrStartPosition();

	unsigned int getCurrPosition();

	void resetCursor();

protected:

	PosFilterBlock* SPFB;
	byte* bfrWithHeader;
	byte* buffer;
	unsigned int* bufferPtrAsIntArr;
	unsigned int* numValues;
	unsigned int* startPos;
	unsigned int* endPos;
	int* posIndex;
	unsigned int currStartPos;
	int currIndexInVal;
	unsigned int currInt;
	unsigned int currPos;
	unsigned int endInt;

	virtual void setPosIndex(int v, int* pidx, int& currIndexInVal);
};

