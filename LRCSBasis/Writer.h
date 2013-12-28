#pragma once
#include "Constants.h"
#include "PosBlock.h"
#include "Operator.h"

class Block;

class  Writer : public Operator {
public:

	Writer(Operator* blockSrc_, int colIndex_);
	virtual ~Writer();

	// Gets the next value block from the operator 
	virtual	Block* getNextValBlock(int colIndex_);

	// Gets the next position block (bitstring of positions) from the operator
	virtual PosBlock* getNextPosBlock(int colIndex_);

	// Gets the maximum position in the data source
	virtual int getLastPosition() = 0;

protected:
	Operator* blockSrc;
	int colIndex;
};

