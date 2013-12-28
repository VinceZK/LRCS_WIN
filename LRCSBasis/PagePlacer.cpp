#include "Util.h"
#include "UncompressedEncoder.h"
#include "UncompressedDecoder.h"
#include <string>
#include <map>
#include <cstdlib>
#include <sstream>
#include "AccessException.h"
#include "PagePlacer.h"


using namespace std;

PagePlacer::PagePlacer(Encoder* src_, Decoder* decoder_, int numIndexes_, bool posPrimary_)
{
	setSrc(src_, decoder_, numIndexes_, posPrimary_);
	lastSecondaryValueSet = false;
	lastSecondaryValue = NULL; // whatever
}

PagePlacer::~PagePlacer()
{
}

void PagePlacer::setSrc(Encoder* src_, Decoder* decoder_, int numIndexes_, bool posPrimary_) {
	encoder = src_;
	decoder = decoder_;
	numIndexes = numIndexes_;

	posPrimary = posPrimary_;

	if (numIndexes != 1 && numIndexes != 2) 
		throw new AccessException("PagePlacer: page needs to have either 1 or 2 indices");
}

unsigned int PagePlacer::placeColumn(string name_, bool splitOnValue_, bool valSorted_, unsigned int lastPos_) {
	valSorted = valSorted_;

	int page_size;
	unsigned int newLastPos = 0;

	Log::writeToLog("PagePlacer", 2, "NOT Spliting on Value");
	PageWriter* pageWriter = NULL;
	byte* page = encoder->getPage();
	while (page != NULL) {
		page_size = encoder->getPageSize();
		encoder->correctStartPos(lastPos_);
		decoder->setBuffer(page);
		newLastPos = decoder->getEndPos();
		if (!pageWriter) {
			int primkeysize = sizeof(int);
			int seckeysize = sizeof(int);
			short primkeytype = ValPos::INTTYPE;
			short seckeytype = ValPos::INTTYPE;
			if (!posPrimary) {
				primkeysize = decoder->getStartVal()->getSize();
				primkeytype = decoder->getStartVal()->type;
			}
			if (posPrimary && numIndexes == 2) {
				seckeysize = decoder->getStartVal()->getSize();
				seckeytype = decoder->getStartVal()->type;
			}
			pageWriter = new PageWriter(numIndexes, primkeysize, seckeysize, primkeytype, seckeytype);
			pageWriter->initDB(name_, (valSorted ? false : true));
		}
		writePage(pageWriter, page, page_size, posPrimary, numIndexes);
		
		//If the encoder has a special position encoder, dry up the position pages
		if (encoder->hasPosEncoder()){
			byte* posValue;
			unsigned int* posPageSize;
			byte* posPage = encoder->getEncodedPosPage(&posValue, &posPageSize);
			while (posPage != NULL){
				pageWriter->placePageSecondary((char*)posPage, (char*)posValue, *posPageSize);
				delete[] posPage;
				delete[] posValue;
				posPage = encoder->getEncodedPosPage(&posValue, &posPageSize);
			}
		}
		
		page = encoder->getPage();
	}

	pageWriter->closeDB();
	delete pageWriter;
	return newLastPos;
}

void PagePlacer::writePage(PageWriter* writer_, byte* page_, int page_size, bool posPrimary_, int numIndexes_) {
	bool useSecondary;
	decoder->setBuffer(page_);

	int temppos;
	temppos = decoder->getEndPos();

	if (numIndexes_ == 1) {
		if (posPrimary_) {
			writer_->placePage((char*)page_, (char*)&temppos, page_size);
		}
		else {
			//assert(valSorted);
			writer_->placePage((char*)page_, (char*)decoder->getEndVal()->value, page_size);
		}

	}
	else if (numIndexes_ == 2) {
		if (valSorted){
			if (posPrimary_) {
				if (!lastSecondaryValueSet) {//Last Secondary Value is not set, create secondary value index
					lastSecondaryValueSet = true;
					lastSecondaryValue = decoder->getEndVal()->clone();
					useSecondary = true;
				}
				else { //Last Secondary Value is set
					ValPos* newSecVal = decoder->getEndVal();
					if (*newSecVal != lastSecondaryValue) {//Different value, create secondary value index
						delete lastSecondaryValue;
						lastSecondaryValue = newSecVal->clone();
						useSecondary = true;
					}
					else    //Same value, continue, do not create
						useSecondary = false;
				}


				if (useSecondary) {
					//temppos = decoder->getEndPos();
					writer_->placePage((char*)page_, (char*)&(temppos), (char*)lastSecondaryValue->value, page_size);
				}
				else {
					//temppos = decoder->getEndPos();
					writer_->placePage((char*)page_, (char*)&(temppos), page_size);
				}

			}
			else {
				//temppos = decoder->getEndPos();
				writer_->placePage((char*)page_, (char*)decoder->getEndVal()->value, (char*)&(temppos), page_size);
			}
		}
		else{//Value is unsorted:Create Position Primary and Value secondary
			//Postion priamry Index
			//temppos = decoder->getEndPos();
			writer_->placePage((char*)page_, (char*)&temppos, page_size);
			//Value secondary Index
			if (!encoder->hasPosEncoder()){//Skip special position encode case
				while (decoder->hasNextBlock()){
					Block* outBlock = decoder->getNextBlock();
					while (outBlock->hasNext()){
						ValPos* vp = outBlock->getNext();
						temppos = vp->position;
						writer_->placeUnsortedRecord((char*)vp->value, (char*)&temppos);
					}
				}
			}
		}
	}
}
