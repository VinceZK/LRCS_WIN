#include "BitmapDecoder.h"


BitmapDecoder::BitmapDecoder() : Decoder() {
	//posBlock = new PosBlock(vp_);
	//posFilterBlock = new PosFilterBlock( );
	//valSorted = true;
}

BitmapDecoder::~BitmapDecoder()
{

}

void BitmapDecoder::setBuffer(byte* buffer_) {
	Decoder::setBuffer(buffer_);
	if (buffer == NULL) throw new CodingException("BitmapDecoder: Error, buffer passed in is NULL");
	numBlocksPtr = (unsigned int*)buffer;
	blockArray = (buffer_ + sizeof(int));
	currBlockNum = 0;
}

int BitmapDecoder::getPageLengthBits() {
	return (8 * ((((unsigned int)*numBlocksPtr) * BLOCK_SIZE) + sizeof(int)));
}

bool BitmapDecoder::hasNextBlock() {
	if (!initialized) return false;
	return ((unsigned int)currBlockNum<*numBlocksPtr);
}

Block* BitmapDecoder::getNextBlock() {
	if (!initialized) return NULL;
	posFilterBlock = new PosFilterBlock();
	posFilterBlock->setBuffer(blockArray + currBlockNum*BLOCK_SIZE);
	currBlockNum++;
	return posFilterBlock;
}
