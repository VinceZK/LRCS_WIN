#pragma once
#include "Encoder.h"
//#include "BasicBlockWriter.h"

using namespace std;

class __declspec(dllexport) LZEncoder :
	public Encoder
{
public:
	LZEncoder(Encoder* encoder_);
	virtual ~LZEncoder();
	virtual byte* getPage();
	//virtual byte* getEncodedPosPage(byte** posValue_, unsigned int** posPageSize_);
	virtual int getNumValsPerPage(){ return 0; } //I don't think it is necessary
	virtual short getValSize(){ return 0; }//I don't think it is necessary
	virtual int getBufferSize(){ return bufferSize; }
	virtual unsigned int getStartPos(){ return 0; }//I don't think it is necessary
	virtual unsigned int getPageSize();
protected:
	int bufferSize;
	byte* page;
	byte* buffer;
	Encoder* encoder;
	unsigned int sizeCompressedData;
};

