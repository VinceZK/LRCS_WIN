#pragma once
#include "Encoder.h"
#include "IntWriter.h"

class __declspec(dllexport) PosEncoder :
	public Encoder
{
public:
	PosEncoder();
	virtual ~PosEncoder();
	virtual byte* getPage(){ return NULL; }
	virtual byte* getPageAndValue(byte** value_, unsigned int** valSize_) = 0;
	virtual int getNumValsPerPage(){ return 0; }
	virtual short getValSize(){ return (short)sizeof(int); }
	virtual int getBufferSize(){ return 0; }
	virtual unsigned int getStartPos(){ return 0; }
	virtual unsigned int getPageSize(){ return PAGE_SIZE; }
	virtual void addValPos(ValPos* vp_) = 0;
	virtual void purgeMap2Queue() = 0;
protected:

};

