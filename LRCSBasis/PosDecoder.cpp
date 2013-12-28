#include "PosDecoder.h"

PosDecoder::PosDecoder(bool valSorted_) : Decoder() {
	outBlock = new MultiBlock(valSorted_, true, true, ValPos::INTTYPE);
	outPair = outBlock->vp;
}
PosDecoder::PosDecoder(byte* buffer_, bool valSorted_) : Decoder(buffer_) {
	outBlock = new MultiBlock(valSorted_, true, true, ValPos::INTTYPE);
	setBuffer(buffer_);
	outPair = outBlock->vp;
}
PosDecoder::PosDecoder(PosDecoder& decoder_) : Decoder(decoder_) {
	reader = decoder_.reader;
	numIntsPtr = decoder_.numIntsPtr;
	startPos = decoder_.startPos;
	outBlock = new MultiBlock(*decoder_.outBlock);
	outPair = outBlock->vp;
}

PosDecoder::~PosDecoder()
{
	if (outBlock)delete outBlock;
}

void PosDecoder::setBuffer(byte* buffer_) {
	Decoder::setBuffer(buffer_);
	if (buffer == NULL) throw new CodingException("PosDecoder: Error, buffer passed in is NULL");
	numIntsPtr = (unsigned int*)buffer_;
	startPos = 1;
	currPos = 1;
	reader.setBuffer(buffer_ + sizeof(int), getPageLengthBits());
}

int PosDecoder::getValueSize(){
	return 8 * sizeof(int);
}
int PosDecoder::getPageLengthBits() {
	return 8 * (*numIntsPtr)*sizeof(int);
}

bool PosDecoder::hasNextBlock() {
	if (!initialized) return false;
	return (currPos - startPos<*numIntsPtr);
}

Block* PosDecoder::getNextBlock() {
	if (!initialized) return NULL;
	int _numInts;
	byte* buffer;
	if (!(buffer = reader.getBuffer(_numInts))) return NULL;
	outBlock->setBuffer(currPos, _numInts, sizeof(int), buffer);
	currPos += _numInts;
	return outBlock;

}

Block* PosDecoder::getNextBlockSingle() {
	if (!initialized) return NULL;
	int value;
	if (!(reader.readInt(value))) return NULL;
	outBlock->setBuffer(currPos, 1, sizeof(int), (byte*)&value);;
	currPos++;
	return outBlock;

}
Block* PosDecoder::peekNextBlock() {
	if (!initialized) return NULL;
	int value;
	int oldPos = reader.getCurrentIntPos();
	if (!reader.readInt(value)) return NULL;
	reader.skipToIntPos(oldPos);
	outPair->set(currPos, (byte*)&value);
	outBlock->currPos = -1;
	return outBlock;

}
bool PosDecoder::skipToPos(unsigned int blockPos_) {
	if (blockPos_ >= *numIntsPtr) return false;//NULL;
	if (reader.skipToIntPos(blockPos_)) {
		currPos = startPos + blockPos_;
		return true;
	}
	else {
		return false;//NULL;	
	}
}

bool PosDecoder::skipToBlockOnValue(ValPos* rhs_) {
	// currently scan down, however can do binary search when sorted in future
	reader.resetPos();
	int value;
	while (reader.readInt(value)) {
		ValPos* lhs = new IntValPos();
		lhs->set((byte*)&value);
		if (*lhs != rhs_) {
			currPos++;
			if (valSorted && *lhs>rhs_)return false;
		}
		else return true;
	}
	return false;
}


ValPos* PosDecoder::getStartVal() {
	delete utilityPair;
	utilityPair = new IntValPos(0, reader.readFirst());
	return utilityPair;
}
unsigned int PosDecoder::getStartPos() {
	return startPos;
}
ValPos* PosDecoder::getEndVal() {
	delete utilityPair;
	utilityPair = new IntValPos(0, reader.readLast());
	return utilityPair;
}
unsigned int PosDecoder::getEndPos() {
	return startPos + (*numIntsPtr) - 1;
}