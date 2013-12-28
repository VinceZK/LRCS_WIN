#include "Writer.h"

Writer::Writer(Operator* blockSrc_, int colIndex_)
{
	blockSrc = blockSrc_;
	colIndex = colIndex_;
}
Writer::~Writer()
{
}

// Gets the next value block from the operator 
Block* Writer::getNextValBlock(int colIndex_) {
	return NULL;
}

// Gets the next position block (bitstring of positions) from the operator
PosBlock* Writer::getNextPosBlock(int colIndex_) {
	return NULL;
}
