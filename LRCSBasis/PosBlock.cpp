#include "PosBlock.h"
using namespace std;
PosBlock::PosBlock(ValPos* vp_) : Block()
{
	posIndex = new int[8 * sizeof(int)+1];
	if (vp_ != NULL){
		vp = vp_;
		valsize = vp->getSize();
	}
	else{
		vp = NULL;
		valsize = 0;
	}
	isBufferSet = false;
	currInt = 0;
	currPos = 0;
	bfrWithHeader = NULL;
	buffer = NULL;
	bufferPtrAsIntArr = NULL;
	numValues = NULL;
	startPos = NULL;
	endPos = NULL;
}

PosBlock::PosBlock(const PosBlock& block_) : Block(block_)
{
	if (block_.vp != NULL)vp = block_.vp->clone();
	else vp = NULL;
	setBuffer(block_.bfrWithHeader);
}

Block* PosBlock::clone(Block& block_) {
	PosBlock* newBlock = new PosBlock((PosBlock&)block_);
	return newBlock;
}


PosBlock::~PosBlock()
{
	if (!isBufferSet && bfrWithHeader){
		delete[] bfrWithHeader;
		bfrWithHeader = NULL;
	}
	delete[] posIndex;
}

void PosBlock::setBuffer(byte* buffer_) {
	//assert(BLOCK_SIZE >= 4 * sizeof(int));
	//assert(buffer_ != NULL);
	isBufferSet = false;
	if (bfrWithHeader == NULL)bfrWithHeader = new byte[BLOCK_SIZE];
	memcpy(bfrWithHeader, buffer_, BLOCK_SIZE);
	init();
}

void PosBlock::setBufferDirect(byte* buffer_) {
	//assert(buffer_ != NULL && sizeof(buffer_) == BLOCK_SIZE);
	isBufferSet = true;
	bfrWithHeader = buffer_;
	init();
}

void PosBlock::initBuffer() {
	//assert(BLOCK_SIZE >= 4 * sizeof(int));
	isBufferSet = false;
	if (bfrWithHeader == NULL)bfrWithHeader = new byte[BLOCK_SIZE];
	memset(bfrWithHeader, 0, BLOCK_SIZE);
	init();
}

void PosBlock::init(){
	startPos = (unsigned int*)bfrWithHeader;
	endPos = (unsigned int*)(bfrWithHeader + sizeof(int));
	numValues = (unsigned int*)(bfrWithHeader + 2 * sizeof(int));

	buffer = bfrWithHeader + (3 * sizeof(int));
	bufferPtrAsIntArr = (unsigned int*)(bfrWithHeader + (2 * sizeof(int)));
	caculateEndInt();
}

ValPos*  PosBlock::getNext() {
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
				return NULL;
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
		return NULL;
	}

	if (vp == NULL) vp = new StringValPos();
	vp->position = currPos;
	return vp;
}

bool PosBlock::addPosition(unsigned int pos) {
	using namespace std;
	unsigned int movBit;
	unsigned int newEndInt;

	if (pos == 0)return false;//illegal position

	if (*startPos == 0){//Add initial value
		*startPos = pos;
		currInt = 1;
		endInt = 1;
		bufferPtrAsIntArr[currInt] |= 1 << 31;
	}
	else{
		////assert(pos > *startPos);//The position is added in order
		if (pos <= *startPos){
			cout << pos << endl;
			cout << *startPos << endl;
		}
		//assert(pos > *startPos);
		newEndInt = (pos - *startPos) / 32 + 1;
		if (newEndInt > MAX_INT) return false;//Block is full
		currInt = newEndInt;
		endInt = newEndInt;
		movBit = (pos - *startPos) % 32;
		bufferPtrAsIntArr[currInt] |= (1 << (31 - movBit));
	}

	*endPos = pos;
	*numValues += 1;
	return true;
}

void PosBlock::resetBlock() {
	if (bfrWithHeader)
		memset(bfrWithHeader, 0, BLOCK_SIZE);
	setCurrInt(0);
}

bool PosBlock::setCurrInt(unsigned int currInt_){
	if (currInt_ > endInt)return false;
	currInt = currInt_;
	currIndexInVal = 0;
	return true;
}

bool PosBlock::isOneValue() {
	return false;
}
bool PosBlock::isValueSorted() {
	return true;
}

bool PosBlock::isPosContiguous() {
	return true;
}
bool PosBlock::isPosSorted() {
	return true;
}
bool PosBlock::isBlockValueSorted() {
	return true;
}
bool PosBlock::isBlockPosSorted() {
	return true;
}

void PosBlock::caculateEndInt() {
	\
	if (*endPos < *startPos){
		endInt = 0;
	}
	else
		endInt = (*endPos - *startPos) / (8 * sizeof(int)) + 1;
}

void PosBlock::setPosIndex(int v, int* pidx, int& currIndexInVal) {
	currIndexInVal = 0;
	for (int i = (8 * sizeof(int)) - 1; i >= 0; i--) {
		if ((1 << i) & v) {
			pidx[currIndexInVal] = ((8 * sizeof(int)) - 1) - i;
			currIndexInVal++;
		}
	}
	pidx[currIndexInVal] = 255;
	currIndexInVal = 0;
}

void PosBlock::printBits(int bits) {
	throw new UnimplementedException("Not implemented");
}

void PosBlock::printBlock() {
	throw new UnimplementedException("Not implemented");
}
