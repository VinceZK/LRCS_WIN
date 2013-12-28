#include "BitBlock.h"
#include "log.h"

BitBlock::BitBlock(bool valSorted_) : Block()
{
	bfr = NULL;
	bfrSize = 0;
	valSorted = valSorted_;
	reader = NULL;
	//outPair=new Pair();
	outPair = new IntValPos();
	fieldSize = 0;
	startPos = 0;
	currLoc = 0;
}

BitBlock::BitBlock(BitBlock& block_) : Block(block_) {
	valSorted = block_.valSorted;
	//outPair=new Pair();
	outPair = new IntValPos();
	bfrSize = block_.bfrSize;
	if (block_.bfr != NULL) {
		memcpy(bfr, block_.bfr, bfrSize);
	}
	else {
		bfr = NULL;
	}

	init = block_.init;
	fieldSize = block_.fieldSize;
	startPos = block_.startPos;
	setBuffer(bfr, bfrSize);
}

Block* BitBlock::clone(Block& block_)
{
	Block* newBlock = new BitBlock((BitBlock&)block_);
	return newBlock;
}

BitBlock::~BitBlock()
{
	if (reader != NULL) delete reader;
	delete outPair;
}
byte* BitBlock::getBuffer() {
	return bfr;
}
void BitBlock::setBuffer(byte* bfr_, int bfrSize_) {
	if (reader == NULL) {
		reader = new BitReader(bfr_ + 3 * sizeof(unsigned int), bfrSize_ - 8 * 3 * sizeof(unsigned int));
	}
	else {
		reader->setBuffer(bfr_ + 3 * sizeof(unsigned int), bfrSize_ - 8 * 3 * sizeof(unsigned int));
	}
	currLoc = 0;
	init = true;
	bfr = bfr_;
	bfrSize = bfrSize_;
	fieldSize = *((unsigned int*)bfr_);
	startPos = *((unsigned int*)(bfr_ + sizeof(int)));
	numValues = *((unsigned int*)(bfr_ + 2 * sizeof(int)));
	Log::writeToLog("BitBlock", 1, "Set buffer, fieldSize", fieldSize);
	Log::writeToLog("BitBlock", 1, "Set buffer, startPos", startPos);
	Log::writeToLog("BitBlock", 1, "Set buffer, numValues", numValues);
}
bool BitBlock::hasNext() {
	if (!init) throw new UnexpectedException("BitBlock: error, not initialized.");
	return (currLoc<numValues);
}
bool BitBlock::hasNext(int value_) {
	if (!hasNext()) return false;
	unsigned int val;
	if (reader->readBits(val, fieldSize)) {
		return (int)val == value_;
	}
	else {
		return false;
	}
}
ValPos* BitBlock::getNext() {
	if (!init) throw new UnexpectedException("BitBlock: error, not initialized.");
	unsigned int val;
	if (!hasNext()) return NULL;
	if (reader->readBits(val, fieldSize)) {
		outPair->set(startPos + currLoc, (byte*)&val);
		currLoc++;
		return outPair;
	}
	throw new CodingException("BitBlock: Error, could not read");
}
ValPos* BitBlock::peekNext() {
	if (!init) throw new UnexpectedException("BitBlock: error, not initialized.");
	unsigned int val;
	if (!hasNext()) return NULL;

	int oldPos = reader->getCurrentBitPosition();
	if (reader->readBits(val, fieldSize)) {
		outPair->set(startPos + currLoc, (byte*)&val);
		reader->skipToBitPos(oldPos);
		return outPair;
	}
	reader->skipToBitPos(oldPos);
	throw new CodingException("BitBlock: Error, could not read");
}

// Zero indexed, gets the pair at this loc
ValPos* BitBlock::getPairAtLoc(unsigned int loc_) {
	//Note, this actually changes the iterator ... this should not happen. fix if DeltaPosBlock ever used ...
	if (!init) throw new UnexpectedException("BitBlock: error, not initialized.");
	if (loc_ >= numValues) return NULL;
	if (reader->skipToBitPos(loc_*fieldSize)) {
		currLoc = loc_;
		return getNext();
	}
	else
		return NULL;
}


//Like getPairAtLoc except set the regular pair variable rather than 
//the utilityPair variable so can be called by getNext().
ValPos* BitBlock::getPairAtLocNotUtility(unsigned int loc_) {
	return getPairAtLoc(loc_);
}

int BitBlock::getCurrLoc() {
	if (!init) throw new UnexpectedException("BitBlock: error, not initialized.");
	return currLoc;
}

unsigned int BitBlock::getFieldSize() {
	return fieldSize;
}
// return size of block		
int BitBlock::getSize() {
	return numValues;
}
int BitBlock::getSizeInBits() {
	return 8 * (((numValues*fieldSize) / 8) + 1);
}
ValPos* BitBlock::getStartPair() {
	if (numValues == 0) return NULL;
	unsigned int oldLoc = currLoc;
	int val = *(int*)getPairAtLoc(0)->value;
	if (oldLoc != 0)
		getPairAtLoc(oldLoc - 1);
	else
		resetBlock();

	outPair->set(startPos, (byte*)&val);
	if (oldLoc != currLoc) throw new UnexpectedException("BitBlock: Miguel you screwed up");
	return outPair;
}
ValPos* BitBlock::getEndPair() {
	if (numValues == 0) return NULL;
	unsigned int oldLoc = currLoc;
	int val = *(int*)getPairAtLoc(getSize() - 1)->value;

	if (oldLoc != 0)
		getPairAtLoc(oldLoc - 1);
	else
		resetBlock();

	outPair->set(startPos + getSize() - 1, (byte*)&val);
	if (oldLoc != currLoc) {
		cout << "Old Loc: " << oldLoc << " CurrLoc: " << currLoc << endl;
		throw new UnexpectedException("BitBlock: Miguel you screwed up");
	}
	return outPair;
}

unsigned int BitBlock::getEndPosition() {
	return getEndPair()->position;
}

void BitBlock::resetBlock() {
	if (!init) return;
	currLoc = 0;
	reader->skipToBitPos(0);
}

bool BitBlock::isOneValue() {
	return false;
}
bool BitBlock::isValueSorted() {
	return valSorted;
}

bool BitBlock::isPosContiguous() {
	return true;
}
bool BitBlock::isPosSorted() {
	return true;
}
bool BitBlock::isBlockValueSorted() {
	return valSorted;
}
bool BitBlock::isBlockPosSorted() {
	return true;
}