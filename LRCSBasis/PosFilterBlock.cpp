#include "PosFilterBlock.h"
#include "PosFilterCursor.h"

using namespace std;
PosFilterBlock::PosFilterBlock() :PosBlock(NULL)
{
}

PosFilterBlock::PosFilterBlock(byte* buffer_) : PosBlock(NULL)
{
	setBufferDirect(buffer_);
}


PosFilterBlock::~PosFilterBlock()
{

}


PosFilterBlock* PosFilterBlock::clone() {
	PosFilterBlock* newBlock = new PosFilterBlock();
	newBlock->initEmptyBuffer(0);
	memcpy(newBlock->bfrWithHeader, bfrWithHeader, BLOCK_SIZE);
	newBlock->caculateEndInt();
	newBlock->setCurrInt(1);
	return newBlock;
}

PosFilterBlock*  PosFilterBlock::cutGetRightAtPos(unsigned int pos_){
	if ((int)(pos_ - *startPos) < 0)return NULL;
	if ((int)(pos_ - *startPos) == 0)return this;
	int cutInt = (pos_ - *startPos) / (8 * sizeof(int));
	if (cutInt >= endInt)return NULL;

	int moveBit = (pos_ - *startPos) % (8 * sizeof(int));

	*startPos = pos_;
	*numValues = 0; //number of values has no use in this case, so just ignore it.

	unsigned int leftInt = endInt - cutInt;

	//memcpy(buffer, buffer+cutInt*sizeof(int), leftInt*sizeof(int));
	for (unsigned int i = 1; i <= leftInt; i++)
		bufferPtrAsIntArr[i] = bufferPtrAsIntArr[i + cutInt];
	endInt = leftInt;
	leftMove(moveBit);
	caculateEndInt();
	if (MAX_INT>endInt)//set all the left ints to zero
		memset(buffer + endInt*sizeof(int), 0, (MAX_INT - endInt)*sizeof(int));
	setCurrInt(1);
	return this;
}

PosFilterBlock*  PosFilterBlock::cutGetLeftAtPos(unsigned int pos_){
	if ((int)(pos_ - *startPos) < 0)return NULL;
	int cutInt = (pos_ - *startPos) / (8 * sizeof(int));
	int moveBit = (pos_ - *startPos) % (8 * sizeof(int));
	if (cutInt >= endInt)return NULL;

	*endPos = pos_;
	*numValues = 0; //number of values has no use in this case, so just ignore it.
	caculateEndInt();
	cutTail();
	if (MAX_INT>endInt)//set all the left ints to zero
		memset(buffer + endInt*sizeof(int), 0, (MAX_INT - endInt)*sizeof(int));
	setCurrInt(1);
	return this;
}

void PosFilterBlock::leftMove(int moveBit_){
	if (moveBit_ <= 0 || moveBit_ > 31)return;
	unsigned int valToOr;
	bufferPtrAsIntArr[1] = bufferPtrAsIntArr[1] << moveBit_;
	for (int i = 2; i <= endInt; i++){
		valToOr = bufferPtrAsIntArr[i] >> (32 - moveBit_);
		bufferPtrAsIntArr[i] = bufferPtrAsIntArr[i] << moveBit_;
		bufferPtrAsIntArr[i - 1] |= valToOr;
	}
}

void PosFilterBlock::cutTail(){
	bufferPtrAsIntArr[endInt] &= 0xffffffffu << (31 - (*endPos - *startPos) % 32);
}

void PosFilterBlock::initEmptyBuffer(unsigned int startPos_) {
	//assert(BLOCK_SIZE >= 4 * sizeof(int));
	bfrWithHeader = new byte[BLOCK_SIZE];
	memset(bfrWithHeader, 0, BLOCK_SIZE);
	init();
	*startPos = startPos_;
	*endPos = startPos_;
	*numValues = 1;
	currStartPos = *startPos;
	currInt = 1;
	endInt = currInt;
	bufferPtrAsIntArr[currInt] = bufferPtrAsIntArr[currInt] | (1 << 31);
}

PosFilterCursor* PosFilterBlock::getCursor() {
	PosFilterCursor* newPFC = new PosFilterCursor(this);
	return newPFC;
}

bool PosFilterBlock::addPosition(unsigned int pos) {
	unsigned int movBit;
	unsigned int valToOr;
	unsigned int newEndInt;

	if (pos == 0)return false;//illegal position

	if (pos < *startPos){
		newEndInt = (*endPos - pos) / 32 + 1;
		if (newEndInt > MAX_INT) return false;//Block is full
		unsigned int avaBits = 31 - (*endPos - *startPos) % 32;
		int gap = newEndInt - endInt;
		endInt = newEndInt;
		movBit = (*startPos - pos) % 32;
		*startPos = pos;

		if (movBit == 0){
			for (int y = endInt; y>gap; y--){
				bufferPtrAsIntArr[y] = bufferPtrAsIntArr[y - gap];
				bufferPtrAsIntArr[y - gap] = 0;
			}
		}
		else{
			if (movBit <= avaBits){
				for (int y = endInt; y>gap; y--){
					bufferPtrAsIntArr[y] = bufferPtrAsIntArr[y - gap] >> movBit;
					if ((y - gap - 1) > 0){
						valToOr = bufferPtrAsIntArr[y - gap - 1] << (32 - movBit);
						bufferPtrAsIntArr[y] |= valToOr;
					}
					if (gap > 0) bufferPtrAsIntArr[y - gap] = 0;
				}
			}
			else{
				for (int y = endInt; y>gap; y--){
					bufferPtrAsIntArr[y] |= bufferPtrAsIntArr[y - gap] << (32 - movBit);
					bufferPtrAsIntArr[y - 1] = bufferPtrAsIntArr[y - gap] >> movBit;
					bufferPtrAsIntArr[y - gap] = 0;
				}
			}
		}
		bufferPtrAsIntArr[1] = bufferPtrAsIntArr[1] | (1 << 31);
		*numValues += 1;
		return true;
	}
	else if (pos == *startPos)return true;
	else if ((pos > *startPos) && (pos < *endPos)){
		currInt = (pos - *startPos) / 32 + 1;
		movBit = (pos - *startPos) % 32;
		if (!((1 << (31 - movBit)) & bufferPtrAsIntArr[currInt])) {//make sure the bit is not 1
			bufferPtrAsIntArr[currInt] |= (1 << (31 - movBit));
			*numValues += 1;
		}
		return true;
	}
	else if (pos == *endPos)return true;
	else{
		newEndInt = (pos - *startPos) / 32 + 1;
		if (newEndInt > MAX_INT) return false;//Block is full
		currInt = newEndInt;
		endInt = newEndInt;
		*endPos = pos;
		movBit = (pos - *startPos) % 32;
		bufferPtrAsIntArr[currInt] |= (1 << (31 - movBit));
		*numValues += 1;
		return true;
	}
}

void PosFilterBlock::setRangePos(unsigned int length){
	unsigned int _maxNumPos = getMaxNumPos();
	//assert(length <= _maxNumPos);
	int _numInts = length / (8 * sizeof(int));
	int _numBitsMod = length % (8 * sizeof(int));
	for (int x = 1; x <= _numInts; x++)
		bufferPtrAsIntArr[x] = 0xFFFFFFFFu;
	if (_numBitsMod > 0){
		bufferPtrAsIntArr[(_numInts + 1)] = 0;
		for (int y = 1; y <= _numBitsMod; y++)
			bufferPtrAsIntArr[(_numInts + 1)] |= (1 << (32 - y));
	}
	*endPos = *startPos + length - 1;
	*numValues = length;
}

unsigned int PosFilterBlock::getNextPos() {
	if (currInt <= endInt) {
		if (currInt == 0)setCurrInt(1);
		if (currIndexInVal) {
			currPos = ((currInt - 1)*sizeof(int)* 8) + *startPos + posIndex[currIndexInVal];
			currIndexInVal++;

			if (posIndex[currIndexInVal] == 255) {
				currInt++;
				currIndexInVal = 0;
			}
		}
		else {
			while ((bufferPtrAsIntArr[currInt] == 0) && (currInt < endInt))
				currInt++;

			if ((currInt == endInt) && ((bufferPtrAsIntArr[currInt] == 0)))
				return 0;
			else
				setPosIndex(bufferPtrAsIntArr[currInt], posIndex, currIndexInVal);

			currPos = ((currInt - 1)*sizeof(int)* 8) + *startPos + posIndex[currIndexInVal];
			currIndexInVal++;
			if (posIndex[currIndexInVal] == 255) {
				currInt++;
				currIndexInVal = 0;
			}
		}
	}
	else{
		currPos = 0;
		currInt = 0;
		return 0;
	}
	return currPos;
}

unsigned int PosFilterBlock::getNextInt() {
	currInt++;
	if (currInt <= endInt) {
		return bufferPtrAsIntArr[currInt];
	}
	else{
		currInt = endInt;//reset currInt to endInt;
		return 0;
	}
}

unsigned int PosFilterBlock::getCurrIntValue() {
	return bufferPtrAsIntArr[currInt];
}

//Append a new int 
bool PosFilterBlock::addInt(unsigned int Int_) {
	if (currInt == MAX_INT)return false;//block is full

	bufferPtrAsIntArr[++currInt] = Int_;
	if (getIntEndPos(Int_) != 0)
		*endPos = *startPos + (currInt - 1) * 8 * sizeof(int)+getIntEndPos(Int_) - 1;
	endInt = currInt;
	return true;
}

unsigned int PosFilterBlock::getIntEndPos(unsigned int Int_) {
	if (Int_ == 0)return 0;
	for (int i = 0; i < 8 * sizeof(int); i++)
	if ((1 << i) & Int_){
		return (8 * sizeof(int)-i);
		break;
	}
}

unsigned int PosFilterBlock::getIntStartPos(unsigned int Int_) {
	if (Int_ == 0)return 0;
	for (int i = (8 * sizeof(int)-1); i >= 0; i--)
	if ((1 << i) & Int_){
		return (8 * sizeof(int)-i);
		break;
	}
}

bool PosFilterBlock::hasNextInt() {
	return (currInt < endInt);
}

unsigned int PosFilterBlock::getStartPosition() {
	return *startPos;
}

unsigned int PosFilterBlock::getEndPosition() {
	return *endPos;
}

unsigned int PosFilterBlock::getNumValues() {
	return *numValues;
}

unsigned int PosFilterBlock::getNumValuesR() {
	*numValues = 0;
	BitUtil::compute_bits_in_16bits();

	for (int i = 1; i <= endInt; i++)
		*numValues += BitUtil::precomputed16_bitcount(bufferPtrAsIntArr[i]);

	return *numValues;
}

unsigned int PosFilterBlock::getMaxNumPos(){
	return MAX_INT*sizeof(int)* 8;
};

unsigned int PosFilterBlock::getCurrPosition() {
	return currPos;
}

unsigned int PosFilterBlock::getCurrStartPosition() {
	return currStartPos;
}

unsigned int PosFilterBlock::getCurrIntStartPos() {
	return (*startPos + (currInt - 1) * 8 * sizeof(int));
}

unsigned int PosFilterBlock::getCurrIntRealStartPos() {
	return (*startPos + (currInt - 1) * 8 * sizeof(int)+getIntStartPos(bufferPtrAsIntArr[currInt]) - 1);
}

void PosFilterBlock::setCurrStartPosition() {
	/*Record the current position to the current start position.
	The purpose is to continue the filtering at pervious break point
	*/
	currStartPos = currPos;
}

