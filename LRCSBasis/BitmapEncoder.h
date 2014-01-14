#pragma once
#include "PosEncoder.h"
#include "PosBlock.h"
#include<queue>
#include<map>

using namespace std;

/*Bitmap Encoder
*encoder positions into bitmaps.
*Only used for positions not values.
*/  
class __declspec(dllexport) BitmapEncoder :
	public PosEncoder
{
public:
	BitmapEncoder(int valSize_, int bfrSizeInBits_);
	virtual ~BitmapEncoder();
	//virtual byte* getPage( ){return NULL;}
	virtual byte* getPageAndValue(byte** value_, unsigned int** valSize_);

	virtual void addValPos(ValPos* vp_);
	virtual void purgeMap2Queue();
protected:
	map<string, byte*>* pageMap;
	map<string, PosBlock*>* blockMap;

	struct ele{
		byte* value;
		byte* page;
	};

	queue<ele>* outPutQ;
	int bufferSizeInBytes;
	short valSize;

	byte* initANewPage(string valueStr);
	void initANewBlock(string valueSt, ValPos* vp_);
	bool appendBlocktoPage(byte* page_, PosBlock* posBlock_);
	void putPage2Queue(byte* page_, string valueStr_);
};

