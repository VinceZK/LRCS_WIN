#include "BitmapEncoder.h"
#include "Log.h"

/*
*	+-----------------------------------+
*	|  numBlocks                        |
*	+-----------------------------------+
*	|Block1(6000byte)-->
*	+-----------------------------------+
*   |Block2(6000byte)-->
*	+-----------------------------------+
*/  

BitmapEncoder::BitmapEncoder(int valSize_, int bfrSizeInBits_) : PosEncoder()
{
	bufferSizeInBytes = bfrSizeInBits_ / 8;
	valSize = valSize_;
	pageMap = new map<string, byte*>;
	blockMap = new map<string, PosBlock*>;
	outPutQ = new queue<ele>;

}

BitmapEncoder::~BitmapEncoder()
{
	delete pageMap;
	delete blockMap;
	delete outPutQ;
}


byte* BitmapEncoder::getPageAndValue(byte** value_, unsigned int** valSize_) {
	if (!outPutQ->empty()){
		ele pair = outPutQ->front();
		outPutQ->pop();
		*value_ = pair.value;
		unsigned int pageSize = BLOCK_SIZE*(*(unsigned int*)pair.page) + sizeof(int);
		Log::writeToLog("BitmapEncoder.cpp", 2, "The page size is ", pageSize);
		*valSize_ = &pageSize;
		return pair.page;
	}
	else return NULL;
}

void BitmapEncoder::addValPos(ValPos* vp_){
	map<string, byte*>::iterator p_it;
	map<string, PosBlock*>::iterator b_it;

	string valueStr(reinterpret_cast<char const*>(vp_->value), vp_->getSize());

	b_it = blockMap->find(valueStr);
	if (b_it == blockMap->end()){//Current vp_ doesn't exist in the block map
		initANewBlock(valueStr, vp_);//Initiate a new block
	}
	else{//vp_ block is exisiting
		if (!b_it->second->addPosition(vp_->position)){//block is full
			/*Try to append the block to corresponding page.
			First check whether there is existing page.
			If no, create one*/
			p_it = pageMap->find(valueStr);
			if (p_it == pageMap->end()){
				initANewPage(valueStr);
				p_it = pageMap->find(valueStr); //Should be found
			}
			//if (!appendBlocktoPage(pageMap[valueStr], blockMap[valueStr]))
			if (!appendBlocktoPage(p_it->second, b_it->second))
			{//page is full
				putPage2Queue(p_it->second, valueStr);
				pageMap->erase(p_it);
				//initANewPage(valueStr);
				//appendBlocktoPage(pageMap[valueStr], blockMap[valueStr]);//Shoud be true
				appendBlocktoPage(initANewPage(valueStr), b_it->second);//Shoud be true
			}
			delete b_it->second;
			blockMap->erase(valueStr);//Erase the block in map
			initANewBlock(valueStr, vp_);//Initate a new one	
		}
	}
}

byte* BitmapEncoder::initANewPage(string valueStr_){	
	//pageMap[valueStr_] = new byte[bufferSizeInBytes];
	byte* buff = new byte[bufferSizeInBytes];
	memset(buff, 0, bufferSizeInBytes);
	pageMap->insert(map<string, byte*>::value_type(valueStr_, buff));
	return buff;
}

void BitmapEncoder::initANewBlock(string valueStr_, ValPos* vp_){
	//blockMap[valueStr_] = new PosBlock(vp_);
	//blockMap[valueStr_]->initBuffer();
	//blockMap[valueStr_]->addPosition(vp_->position);
	PosBlock* posB = new PosBlock(vp_);
	posB->initBuffer();
	posB->addPosition(vp_->position);
	blockMap->insert(map<string, PosBlock*>::value_type(valueStr_, posB));
}

bool BitmapEncoder::appendBlocktoPage(byte* page_, PosBlock* posBlock_){
	unsigned int* numBlocks = (unsigned int*)page_;
	if ((*numBlocks + 1)*BLOCK_SIZE > (unsigned int)(bufferSizeInBytes - sizeof(int)))
		return false;//Page is full
	memcpy(page_ + (sizeof(int)+(*numBlocks)*BLOCK_SIZE), posBlock_->getBuffer(), BLOCK_SIZE);
	*numBlocks += 1;
	//delete posBlock_;
	return true;
}

void BitmapEncoder::putPage2Queue(byte* page_, string valueStr_){
	ele pair;
	pair.value = new byte[valueStr_.size() + 1];
	//strcpy((char*)pair.value, valueStr_.c_str()); //Win byte and char are unconvertable!!
	valueStr_.copy((char*)pair.value, valueStr_.size(), 0);
	//pair.value = (byte*)const_cast<char*>(valueStr_.c_str());
	pair.page = page_;
	outPutQ->push(pair);
}

void BitmapEncoder::purgeMap2Queue(){
	map<string, PosBlock*>::iterator b_it;
	map<string, byte*>::iterator p_it;

	for (p_it = pageMap->begin(); p_it != pageMap->end(); p_it++){
		b_it = blockMap->find(p_it->first);
		if (b_it != blockMap->end()){
			if (appendBlocktoPage(p_it->second, b_it->second)){
				putPage2Queue(p_it->second, p_it->first);
			}
			else{//create a new page
				putPage2Queue(p_it->second, p_it->first);
				byte* _page = new byte[bufferSizeInBytes];
				memset(_page, 0, bufferSizeInBytes);
				appendBlocktoPage(_page, b_it->second);
				putPage2Queue(_page, b_it->first);
			}
		}
		else
			putPage2Queue(p_it->second, p_it->first);
	}

	pageMap->clear();
	blockMap->clear();
}