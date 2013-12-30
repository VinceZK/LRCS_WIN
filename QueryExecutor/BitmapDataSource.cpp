#include "BitmapDataSource.h"
#include "StringUtil.h"
#include "LZDecoder.h"
#include "BitmapDecoder.h"
#include "PosOperator.h"

BitmapDataSource::BitmapDataSource(AM* am_, bool valSorted_, bool isROS, Decoder* decoder_)
: DataSource(am_, isROS)
{
	m_bValSorted = valSorted_;
	m_pDecoder = new LZDecoder(decoder_);
	//decoder = new StringDecoder(true);
}


BitmapDataSource::~BitmapDataSource()
{

	if (m_pDecoder != NULL) delete m_pDecoder;
	//if (currBlock!=NULL) delete currBlock;
}

//Get the position block on predicaiton
MultiPosFilterBlock* BitmapDataSource::getPosOnPred(){
	if (m_pPred == NULL){
		m_pMatchedPredPos = new MultiPosFilterBlock();
		m_pMatchedPredPos->setCompleteSet(true);
	}
	else{
		m_bPredChanged = false;//Reset predChanged

		ValPos* rhsvp = m_pPred->getRHS();
		char* rhsval = (char*)rhsvp->value;
		ValPos* tempVP = rhsvp->clone();
		int valsize = m_pPred->getRHS()->getSize();
		unsigned char* temp;
		temp = StringUtil::getSmallestLargerValue(rhsval, valsize);
		tempVP->set(temp);
		// The column is for sure not value sorted
		if (!getPosOnPredValueUnsorted((ROSAM*)m_pAM, rhsvp, tempVP))
			//If nothing find, matched predication position is a NULL set.
			m_pMatchedPredPos = new MultiPosFilterBlock();
	}
	return m_pMatchedPredPos;
}

//Get position on predition if the value is unsorted:check
bool BitmapDataSource::getPosOnPredValueUnsorted(ROSAM* am_, ValPos* rhsvp_, ValPos* tempVP_){
	byte* page = NULL;
	BitmapDecoder* bitmapDecoder = new BitmapDecoder();
	PosOperator* posOperator = new PosOperator();
	MultiPosFilterBlock* multiPosBlock;
	bool done = false;
	//ValPos* pair;

	//assert(pred != NULL);
	char* rhsval = (char*)rhsvp_->value;
	char* temp = (char*)tempVP_->value;

	while (!done){
		//01. get a page from BDB
		switch (m_pPred->getPredType()) {
		case Predicate::OP_GREATER_THAN:
			if (m_bFirstCall)
				page = (byte*)am_->getDbPageRange(temp);
			else
				page = (byte*)am_->getDbPageNext();
			break;
		case Predicate::OP_GREATER_THAN_OR_EQUAL:
			if (m_bFirstCall)
				page = (byte*)am_->getDbPageRange(rhsval);
			else
				page = (byte*)am_->getDbPageNext();
			break;
		case Predicate::OP_EQUAL:
			if (m_bFirstCall)
				page = (byte*)am_->getDbPageSet(rhsval);
			else
				page = (byte*)am_->getDbPageNextDup();
			break;
		case Predicate::OP_LESS_THAN:
			page = (byte*)am_->getDbPageNextRange(rhsval, m_bFirstCall);
			break;
		case Predicate::OP_LESS_THAN_OR_EQUAL:
			page = (byte*)am_->getDbPageNextRange(temp, m_bFirstCall);
			break;
		default:
			break;
		}

		if (page == NULL){
			done = true;
			break;
		}

		//02. Add OR operator
		if (!m_bFirstCall)posOperator->addWhereOp('|');

		//03. Decode the page
		bitmapDecoder->setBuffer(page);
		PosFilterBlock* posFilterBlock = NULL;

		//04. Check out the position block directly to position filters
		multiPosBlock = new MultiPosFilterBlock();
		while (bitmapDecoder->hasNextBlock()){
			posFilterBlock = (PosFilterBlock*)bitmapDecoder->getNextBlock();
			multiPosBlock->addPosFilterBlock(posFilterBlock);
		}

		posOperator->addPosBlock(multiPosBlock);
		m_bFirstCall = false;
	}
	posOperator->finishWhereOp();
	m_pMatchedPredPos = posOperator->getPosFilter();
	//cout<<matchedPredPos->getNumValuesR()<<endl;
	delete bitmapDecoder;
	delete posOperator;

	if (m_pMatchedPredPos == NULL)return false;
	else return true;
}

// Finds the right block on the page
bool BitmapDataSource::skipToRightPosOnPage(unsigned int pos_) {
	if (m_pDecoder->skipToPos(pos_))
		return true;
	else
		return false;
}

