#pragma once
#include "Encoder.h"
#include "Decoder.h"
#include "PageWriter.h"
#include "BDBEnv.h"

using namespace std;

class  __declspec(dllexport) PagePlacer
{
public:
	PagePlacer(Encoder* src_, Decoder* decoder, int numIndexes_, bool posPrimary_);
	virtual ~PagePlacer();
	void setSrc(Encoder* src_, Decoder* decoder_, int numIndexes_, bool posPrimary_);
	unsigned int placeColumn(string name_, bool splitOnValue_, bool valSorted_, unsigned int lastPos_);

protected:
	void writePage(PageWriter* writer_, byte* page_, int page_size, bool posPrimary_, int numIndexes_);
	Encoder* encoder;
	Decoder* decoder;
	int numIndexes;
	bool posPrimary;
	//map<int,PageWriter*> valueMap;
	//map<int,string> nameMap;
	ValPos* lastSecondaryValue;
	bool lastSecondaryValueSet;
	bool valSorted;
	//BDBEnv* bdbenv;
	//COLUMN_STATUS* colStatus;
};

