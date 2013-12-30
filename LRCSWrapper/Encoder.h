#pragma once
#include "Operator.h"
#include "ValPos.h"
#include "UnexpectedException.h"
class PosEncoder;

class __declspec(dllexport) Encoder
{
public:
	Encoder(Operator* dataSrc_, int colIndex_, PosEncoder* posEncoder_);
	virtual ~Encoder();
	virtual byte* getPage();
	virtual byte* getEncodedPosPage(byte** posValue_, unsigned int** posPageSize_);
	virtual bool writeVal(int val_, unsigned int pos_);
	virtual ValPos** getValueIndex() { throw new UnexpectedException("Should only be called for Type2 style encoders"); }
	virtual int getNumValsPerPage() = 0;
	virtual short getValSize() = 0;
	virtual int getBufferSize() = 0;
	virtual unsigned int getStartPos() = 0;
	virtual unsigned int getPageSize() = 0;
	virtual bool hasPosEncoder(){ return(posEncoder == NULL ? false : true); }

	PosEncoder* posEncoder;
protected:
	enum Mode { UNINIT, PULL, PUSH };
	Mode mode;
	Operator* dataSrc;
	int colIndex;
};

