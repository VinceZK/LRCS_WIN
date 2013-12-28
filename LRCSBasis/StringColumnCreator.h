#pragma once
#include "ColumnCreator.h"
#include "SColumnExtracter.h"
#include "StringDecoder.h"
#include "StringEncoder.h"
#include "LZEncoder.h"
#include "LZDecoder.h"

class StringColumnCreator :
	public ColumnCreator
{
public:
	StringColumnCreator(INDEX_QUEUE* idxQ_, unsigned int lastPos_);
	virtual ~StringColumnCreator();
	virtual unsigned int generateColumn();
protected:
	SColumnExtracter* ce;
	StringEncoder* encoder;
	StringDecoder* decoder;
	LZEncoder* lzEncoder;
	LZDecoder* lzDecoder;
};

