#include "IntDecoder.h"

IntDecoder::IntDecoder(bool valSorted_) : Decoder() {
	outBlock = new MultiBlock(valSorted_, true, true, ValPos::INTTYPE);
	outPair = outBlock->vp;
}
IntDecoder::IntDecoder(byte* buffer_, bool valSorted_) : Decoder(buffer_) {
	outBlock = new MultiBlock(valSorted_, true, true, ValPos::INTTYPE);
	setBuffer(buffer_);
	outPair = outBlock->vp;
}
IntDecoder::IntDecoder(IntDecoder& decoder_) : Decoder(decoder_) {
	reader = decoder_.reader;
	numIntsPtr = decoder_.numIntsPtr;
	startPosPtr = decoder_.startPosPtr;
	outBlock = new MultiBlock(*decoder_.outBlock);
	outPair = outBlock->vp;
}

IntDecoder::~IntDecoder()
{
	delete outBlock;
}

void IntDecoder::setBuffer(byte* buffer_) {
	Decoder::setBuffer(buffer_);
	if (buffer == NULL) throw new CodingException("IntDecoder: Error, buffer passed in is NULL");
	numIntsPtr = (unsigned int*)buffer_;
	startPosPtr = (unsigned int*)(buffer_ + sizeof(int));
	currPos = *startPosPtr;
	Log::writeToLog("IntDecoder", 1, "setBuffer(): buffer has numInts=", *numIntsPtr);
	Log::writeToLog("IntDecoder", 1, "setBuffer(): buffer has startPos=", *startPosPtr);

	reader.setBuffer(buffer_ + 2 * sizeof(int), getPageLengthBits() - 8 * 2 * sizeof(int));
}

int IntDecoder::getValueSize(){
	return 8 * sizeof(int);
}
int IntDecoder::getPageLengthBits() {
	Log::writeToLog("IntDecoder", 0, "getPageLength returning leng=", 8 * (2 + ((unsigned int)*numIntsPtr))*sizeof(int));
	return 8 * (2 + ((unsigned int)*numIntsPtr))*sizeof(int);
}

bool IntDecoder::hasNextBlock() {
	if (!initialized) return false;
	return (currPos - *startPosPtr<*numIntsPtr);
}

Block* IntDecoder::getNextBlock() {
	if (!initialized) return NULL;
	int numInts;
	byte* buffer;
	if (!(buffer = reader.getBuffer(numInts))) return NULL;
	outBlock->setBuffer(currPos, numInts, sizeof(int), buffer);
	currPos += numInts;
	return outBlock;

}

Block* IntDecoder::getNextBlockSingle() {
	if (!initialized) return NULL;
	int value;
	if (!(reader.readInt(value))) return NULL;
	//outPair->value=value;
	//outPair->position=currPos;
	//outBlock->currPos=-1;
	outBlock->setBuffer(currPos, 1, sizeof(int), (byte*)&value);
	//	outBlock->setValue(value, currPos);
	currPos++;
	// Log::writeToLog("IntDecoder", 0, "getNext returning pair value", value);
	return outBlock;

}
Block* IntDecoder::peekNextBlock() {
	if (!initialized) return NULL;
	int value;
	int oldPos = reader.getCurrentIntPos();
	if (!reader.readInt(value)) return NULL;
	reader.skipToIntPos(oldPos);
	outPair->set(currPos, (byte*)&value);
	//outPair->value=value;
	//outPair->position=currPos;
	outBlock->currPos = -1;
	return outBlock;

}
bool IntDecoder::skipToPos(unsigned int blockPos_) {
	if (blockPos_ >= *numIntsPtr) return false;//NULL;
	if (reader.skipToIntPos(blockPos_)) {
		currPos = *startPosPtr + blockPos_;
		return true;
	}
	else {
		return false;//NULL;	
	}
}

bool IntDecoder::skipToBlockOnValue(ValPos* rhs_) {
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


ValPos* IntDecoder::getStartVal() {
	delete utilityPair;
	utilityPair = new IntValPos(0, reader.readFirst());
	return utilityPair;
}
unsigned int IntDecoder::getStartPos() {
	return *startPosPtr;
}
ValPos* IntDecoder::getEndVal() {
	delete utilityPair;
	utilityPair = new IntValPos(0, reader.readLast());
	return utilityPair;
}
unsigned int IntDecoder::getEndPos() {
	return (*startPosPtr) + (*numIntsPtr) - 1;
}
