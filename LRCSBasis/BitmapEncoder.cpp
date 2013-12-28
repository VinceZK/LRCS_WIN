#include "BitmapEncoder.h"

BitmapEncoder::BitmapEncoder(int valSize_, int bfrSizeInBits_) : PosEncoder()
{
	bufferSizeInBytes = bfrSizeInBits_ / 8;
	valSize = valSize_;
}

BitmapEncoder::~BitmapEncoder()
{

}


byte* BitmapEncoder::getPageAndValue(byte** value_, unsigned int** valSize_) {
	if (!outPutQ.empty()){
		ele pair = outPutQ.front();
		outPutQ.pop();
		*value_ = pair.value;
		unsigned int pageSize = BLOCK_SIZE*(*(unsigned int*)pair.page) + sizeof(int);
		*valSize_ = &pageSize;
		return pair.page;
	}
	else return NULL;
}

void BitmapEncoder::addValPos(ValPos* vp_){
	map<string, byte*>::iterator p_it;
	map<string, PosBlock*>::iterator b_it;

	string valueStr(reinterpret_cast<char const*>(vp_->value), vp_->getSize());

	b_it = blockMap.find(valueStr);
	if (b_it == blockMap.end()){//Current vp_ doesn't exist in the block map
		initANewBlock(valueStr, vp_);//Initiate a new block
	}
	else{//vp_ block is exisiting
		if (!blockMap[valueStr]->addPosition(vp_->position)){//block is full
			/*Try to append the block to corresponding page.
			First check whether there is existing page.
			If no, create one*/
			p_it = pageMap.find(valueStr);
			if (p_it == pageMap.end())initANewPage(valueStr);
			if (!appendBlocktoPage(pageMap[valueStr], blockMap[valueStr]))
			{//page is full
				putPage2Queue(pageMap[valueStr], valueStr);
				pageMap.erase(p_it);
				initANewPage(valueStr);
				appendBlocktoPage(pageMap[valueStr], blockMap[valueStr]);//Shoud be true
			}
			delete blockMap[valueStr];
			blockMap.erase(valueStr);//Erase the block in map
			initANewBlock(valueStr, vp_);//Initate a new one	
		}
	}
}

void BitmapEncoder::initANewPage(string valueStr_){
	pageMap[valueStr_] = new byte[bufferSizeInBytes];
	memset(pageMap[valueStr_], 0, bufferSizeInBytes);
}

void BitmapEncoder::initANewBlock(string valueStr_, ValPos* vp_){
	blockMap[valueStr_] = new PosBlock(vp_);
	blockMap[valueStr_]->initBuffer();
	blockMap[valueStr_]->addPosition(vp_->position);
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
	outPutQ.push(pair);
}

void BitmapEncoder::purgeMap2Queue(){
	map<string, PosBlock*>::iterator b_it;
	map<string, byte*>::iterator p_it;

	for (p_it = pageMap.begin(); p_it != pageMap.end(); p_it++){
		b_it = blockMap.find(p_it->first);
		if (b_it != blockMap.end()){
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

	pageMap.clear();
	blockMap.clear();
}