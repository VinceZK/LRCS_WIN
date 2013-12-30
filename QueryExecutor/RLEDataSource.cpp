#include "RLEDataSource.h"
#include "StringUtil.h"
#include "LZDecoder.h"
#include "RLEDecoder.h"
#include "PosOperator.h"

RLEDataSource::RLEDataSource(AM* am_, bool valSorted_, bool isROS, Decoder* decoder_)
: DataSource(am_, isROS)
{
	m_bValSorted = valSorted_;
	m_pDecoder = new LZDecoder(decoder_);
	//decoder = new StringDecoder(true);
}


RLEDataSource::~RLEDataSource()
{

	if (m_pDecoder != NULL) delete m_pDecoder;
	//if (currBlock!=NULL) delete currBlock;
}

//Get the position block on predicaiton
MultiPosFilterBlock* RLEDataSource::getPosOnPred(){
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
bool RLEDataSource::getPosOnPredValueUnsorted(ROSAM* am_, ValPos* rhsvp_, ValPos* tempVP_){
	byte* page = NULL;
	RLEDecoder* _RLEDecoder = new RLEDecoder(false, true);

	bool done = false;

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

		//02. Decode the page
		_RLEDecoder->setBuffer(page);

		//03. Check out the position block directly to position filters
		if (m_pMatchedPredPos == NULL)
			m_pMatchedPredPos = new MultiPosFilterBlock();
		RLEBlock* posFilterBlock = (RLEBlock*)_RLEDecoder->getNextBlock();
		m_pMatchedPredPos->addRLEBlock(posFilterBlock);

		m_bFirstCall = false;
	}

	delete _RLEDecoder;

	if (m_pMatchedPredPos == NULL)return false;
	else return true;
}

// Finds the right block on the page
/*bool RLEDataSource::skipToRightPosOnPage(unsigned int pos_) {
if (decoder->skipToPos(pos_))
return true;
else
return false;
}*/
