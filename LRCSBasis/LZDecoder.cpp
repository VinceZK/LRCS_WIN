#include "LZDecoder.h"
#include "Util.h"
#include <lzo1x.h>
#include <lzoutil.h>

LZDecoder::LZDecoder(Decoder* decoder_) : Decoder() {
	//assert(decoder_ != NULL);
	decoder = decoder_;
}

LZDecoder::~LZDecoder() {
}

void LZDecoder::setBuffer(byte* buffer_) {
	if (buffer_ == NULL)
		throw new CodingException("LZDecoder: Error, buffer passed in is NULL");
	sizeCompressed = *((unsigned int*)(buffer_));
	sizeOrignal = *((unsigned int*)(buffer_ + sizeof(int)));

	byte* data = buffer_ + 2 * sizeof(int);
	lzo_uint new_len;
	lzo_byte* decompressedBuffer = (lzo_bytep)lzo_malloc(sizeOrignal);
	int r = lzo1x_decompress(data, sizeCompressed, decompressedBuffer, &new_len, NULL);
	if (r == LZO_E_OK && new_len == sizeOrignal)
		Log::writeToLog("LZDecoder", 1, "Decompress OK!");
	else
		throw new UnexpectedException("LZDecoder: Decompress Error!");

	//Decoder::setBuffer(decompressedBuffer);			
	decoder->setBuffer(decompressedBuffer);

}

bool LZDecoder::hasNextBlock() {
	return decoder->hasNextBlock();
}

Block* LZDecoder::getNextBlock() {
	return decoder->getNextBlock();
}

Block* LZDecoder::getNextBlockSingle() {
	return decoder->getNextBlockSingle();
}

bool LZDecoder::skipToPos(unsigned int blockPos_) {
	return decoder->skipToPos(blockPos_);
}

bool LZDecoder::skipToBlockOnValue(ValPos* rhs_) {
	return decoder->skipToBlockOnValue(rhs_);
}

ValPos* LZDecoder::getStartVal() {
	return decoder->getStartVal();
}
unsigned int LZDecoder::getStartPos() {
	return decoder->getStartPos();
}
ValPos* LZDecoder::getEndVal() {
	return decoder->getEndVal();
}
unsigned int LZDecoder::getEndPos() {
	return decoder->getEndPos();
}
