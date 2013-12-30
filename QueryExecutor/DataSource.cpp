#include "DataSource.h"
#include "StringUtil.h"

using namespace std;

DataSource::DataSource(AM* am_, bool isROS_) {
	init(am_, isROS_);
}

DataSource::DataSource(AM* am_, bool isROS_, bool valSorted_, Decoder* decoder_) {
	init(am_, isROS_);
	m_bValSorted = valSorted_;
	m_pDecoder = decoder_;
	m_pCurrBlock = NULL;

}

void DataSource::init(AM* am_, bool isROS_) {
	if (am_ == NULL)
		throw new UnexpectedException("DataSource::constructor Error, am is NULL");
	m_pAM = am_;

	if (!isROS_)
		m_bPosPrimaryIndex = false;
	else {
		m_bPosPrimaryIndex = true;
	}
	m_bIsROS = isROS_;

	m_pAM->initCursors();
	m_bPosFilterChanged = true;
	m_bPredChanged = true;
	m_pPred = NULL;

	m_pPosFilter = NULL;
	m_pFilterCursor = NULL;
	m_pMatchedPredPos = NULL;
	m_pPosOutTripleOnPred = new RLETriple();
	m_bFirstCall = true;
}

DataSource::~DataSource() {
	if (m_pPosOutTripleOnPred){
		delete m_pPosOutTripleOnPred;
		m_pPosOutTripleOnPred = NULL;
	}
	if (m_pMatchedPredPos){
		delete m_pMatchedPredPos;
		m_pMatchedPredPos = NULL;
	}
	if (m_pFilterCursor){
		delete m_pFilterCursor;
		m_pFilterCursor = NULL;
	}
}

// Changes the RHS binding for this datasource
void DataSource::changeRHSBinding(ValPos* rhs_) {
	if (m_pPred == NULL)
		throw new UnexpectedException("DataSource: Error, setting binding on NULL predicate");
	else {
		m_pPred->setRHS(rhs_);
		m_bPredChanged = true;
	}
}

// sets a new predicate for this datasource
void DataSource::setPredicate(Predicate* pred_) {
	m_bPredChanged = true;
	m_pPred = pred_;
}

// Sets a filter for postions
void DataSource::setPositionFilter(MultiPosFilterBlock* bitstringDataSource_) {
	m_bPredChanged = true;
	m_pPosFilter = bitstringDataSource_;
}


//getLastPosition returns the last position in the data source (column)
int DataSource::getLastPosition() {
	if (m_bPosPrimaryIndex)
		return *(int*)m_pAM->getLastIndexValuePrimary();
	else
		return 1; //To be implemented
}

const void* DataSource::getNextPageValue() {
	if (m_bPosPrimaryIndex)
		return m_pAM->getNextPagePrimary();
	else
		return NULL;//To be implemented
}

const void* DataSource::skipToPageValue(char* key) {
	if (m_bPosPrimaryIndex)
		return m_pAM->skipToPageSecondary(key);
	else
		return m_pAM->skipToPagePrimary(key);
}

const void* DataSource::skipToPagePosition(int key) {
	if (m_bPosPrimaryIndex)
		return m_pAM->skipToPagePrimary((char*)&key);
	else
		return NULL;//To be implemented
}

Block* DataSource::getDecodedBlock() {
	return m_pDecoder->getNextBlock();
}

Block* DataSource::getDecodedBlock(Decoder* decoder_) {
	return decoder_->getNextBlock();
}
// Gets the next value block from the operator 
Block* DataSource::getNextValBlock(int colIndex_) {
	//zklee: Each page is converted to a multiblock, no loop required anymore!
	if (colIndex_ < 0)return NULL;

	byte* page = getRightPage();
	if (page == NULL) return NULL;
	m_pDecoder->setBuffer(page);
	if (m_pPosFilter != NULL){
		unsigned int currStartPos = m_pFilterCursor->getCurrStartPosition();
		skipToRightPosOnPage(currStartPos);
		m_pCurrBlock = (MultiBlock*)getDecodedBlock();
		if (m_pCurrBlock == NULL) return NULL;
		m_pCurrBlock->filterWithPos(m_pFilterCursor);
	}
	else{
		m_pCurrBlock = (MultiBlock*)getDecodedBlock();
	}

	return m_pCurrBlock;
}

byte* DataSource::getRightPage() {
	Log::writeToLog("DataSource", 0, "Called GetRightPage()");
	// if anything has changed we need to reset the indices
	if (m_bPosFilterChanged || m_bPredChanged) {
		Log::writeToLog("Datasource", 0, "Filter or pred changed, initing cursors");
		m_pAM->initCursors();
	}

	if (m_pPosFilter != NULL){
		if (m_pPosFilter->isCompleteSet()){
			return getPage();//Sequential Reading
		}
		else if (m_pPosFilter->isNullSet())
			return NULL;
		else
			return getPageOnPos();
	}
	else
		return getPage();//Sequential Reading
}

byte* DataSource::getPage() {
	return (byte*)getNextPageValue();
}

byte* DataSource::getPageOnPos() {
	if (m_pFilterCursor == NULL){
		m_pFilterCursor = m_pPosFilter->getCursor();
		m_pFilterCursor->getNext();//Move to the start position
		m_pFilterCursor->setCurrStartPosition(); //Record current start position
	}

	if (!m_pFilterCursor->isFilterFinished()){
		m_pAM->initCursors();
		return (byte*)skipToPagePosition(m_pFilterCursor->getCurrStartPosition());
	}
	else return NULL;
}

// Finds the right block on the page
bool DataSource::skipToRightPosOnPage(unsigned int pos_) {
	if (m_pDecoder->skipToPos(pos_))
		return true;
	else
		return false;
}

//Get the position block on predicaiton
MultiPosFilterBlock* DataSource::getPosOnPred(){
	/*
	* This method should be rewritten in children class
	* For the reason of different value types.
	* The default here is string.
	*/
	m_pMatchedPredPos = new MultiPosFilterBlock();
	if (m_pPred == NULL)m_pMatchedPredPos->setCompleteSet(true);
	else{
		m_bPredChanged = false;//Reset predChanged

		ValPos* rhsvp = m_pPred->getRHS();
		char* rhsval = (char*)rhsvp->value;
		ValPos* tempVP = rhsvp->clone();
		int valsize = m_pPred->getRHS()->getSize();
#ifdef DEBUG
		char* pTmp = new char[valsize+1];
		memset(pTmp,  0, sizeof(char)*(valsize+1));
		strncpy(pTmp, rhsval, valsize);
#endif
		unsigned char* temp;
		if (m_pPred->getRHS()->type == ValPos::STRINGTYPE){
			temp = StringUtil::getSmallestLargerValue(rhsval, valsize);
			tempVP->set(temp);
		}
		else{//Default is string type column.
			return NULL;
		}

		if (m_bValSorted) {
			getPosOnPredValueSorted(rhsvp, tempVP);
		}
		else {// The column is not value sorted
			getPosOnPredValueUnsorted((ROSAM*)m_pAM, rhsvp, tempVP);
		}
	}
	return m_pMatchedPredPos;
}
//Get postion on predition if the value is sorted:Check
bool DataSource::getPosOnPredValueSorted(ValPos* rhsvp_, ValPos* tempVP_){

	if (!m_pPosOutTripleOnPred->isNull()) return false;
	byte* page = NULL;
	unsigned int position = 0;
	unsigned int minPos = 1;
	unsigned int maxPos = getLastPosition();

	//assert((rhsvp_ != NULL) && (tempVP_ != NULL));
	char* rhsval = (char*)rhsvp_->value;
	char* temp = (char*)tempVP_->value;

	switch (m_pPred->getPredType()) {
	case Predicate::OP_GREATER_THAN:
		page = (byte*)skipToPageValue(temp);
		if (page != NULL) {
			m_pDecoder->setBuffer(page);
			if (m_pDecoder->skipToBlockOnValue(tempVP_))
				position = ((MultiBlock*)getDecodedBlock(m_pDecoder))->getPosition();
		}
		else return false;
		if (position == 0) m_pPosOutTripleOnPred->setTriple(NULL, minPos, maxPos - minPos + 1);
		if (position == maxPos)return false;
		else
			m_pPosOutTripleOnPred->setTriple(NULL, position, maxPos - position + 1);
		break;
	case Predicate::OP_GREATER_THAN_OR_EQUAL:
		page = (byte*)skipToPageValue(rhsval);
		if (page != NULL) {
			m_pDecoder->setBuffer(page);
			if (m_pDecoder->skipToBlockOnValue(rhsvp_))
				position = ((MultiBlock*)getDecodedBlock(m_pDecoder))->getPosition();
		}
		if (position == 0) m_pMatchedPredPos->setCompleteSet(true);
		if (position == maxPos)return false;
		else m_pPosOutTripleOnPred->setTriple(NULL, position, maxPos - position + 1);
		break;
	case Predicate::OP_EQUAL:
		page = (byte*)skipToPageValue(rhsval);
		if (page != NULL) {
			m_pDecoder->setBuffer(page);
			if (m_pDecoder->skipToBlockOnValue(rhsvp_))
				position = ((MultiBlock*)getDecodedBlock(m_pDecoder))->getPosition();
		}
		if (position == 0)return false;
		unsigned int end;
		page = (byte*)skipToPageValue(temp);
		if (page != NULL) {
			m_pDecoder->setBuffer(page);
			if (m_pDecoder->skipToBlockOnValue(tempVP_))
				end = ((MultiBlock*)getDecodedBlock(m_pDecoder))->getPosition() - 1;
		}
		else end = 0;

		if (end == 0){
			if (position == 1)m_pMatchedPredPos->setCompleteSet(true);
			else m_pPosOutTripleOnPred->setTriple(NULL, position, maxPos - position + 1);
		}
		else m_pPosOutTripleOnPred->setTriple(NULL, position, end - position + 1);
		break;
	case Predicate::OP_LESS_THAN:
		//if (position==minPos) return false;
		page = (byte*)skipToPageValue(rhsval);
		if (page != NULL) {
			m_pDecoder->setBuffer(page);
			if (m_pDecoder->skipToBlockOnValue(rhsvp_))
				position = ((MultiBlock*)getDecodedBlock(m_pDecoder))->getPosition();
		}
		else m_pMatchedPredPos->setCompleteSet(true);
		if (position >= 1 && !m_pMatchedPredPos->isCompleteSet())
			m_pPosOutTripleOnPred->setTriple(NULL, minPos, position - minPos + 1);
		break;
	case Predicate::OP_LESS_THAN_OR_EQUAL:
		//if (position==minPos)return false;
		page = (byte*)skipToPageValue(temp);
		if (page != NULL) {
			m_pDecoder->setBuffer(page);
			if (m_pDecoder->skipToBlockOnValue(tempVP_))
				position = ((MultiBlock*)getDecodedBlock(m_pDecoder))->getPosition();
		}
		else m_pMatchedPredPos->setCompleteSet(true);
		if (position >= 1 && !m_pMatchedPredPos->isCompleteSet())
			m_pPosOutTripleOnPred->setTriple(NULL, minPos, position - minPos + 1);
		break;
	default:
		return false;
		break;
	}

	if (!m_pMatchedPredPos->isCompleteSet())
		m_pMatchedPredPos->setTriple(m_pPosOutTripleOnPred);
	return true;
}

//Get position on predition if the value is unsorted:check
bool DataSource::getPosOnPredValueUnsorted(ROSAM* am_, ValPos* rhsvp_, ValPos* tempVP_){
	byte* page = NULL;
	PosDecoder* posdecoder = new PosDecoder(false);
	bool done = false;
	ValPos* pair;

	//assert(pred != NULL);
	char* rhsval = (char*)rhsvp_->value;
	char* temp = (char*)tempVP_->value;

	while (!done){
		//01. get a page from BDB
		switch (m_pPred->getPredType()) {
		case Predicate::OP_GREATER_THAN:
			if (m_bFirstCall)
				page = (byte*)am_->getDbSetRange(temp);
			else
				page = (byte*)am_->getDbNext();
			break;
		case Predicate::OP_GREATER_THAN_OR_EQUAL:
			if (m_bFirstCall)
				page = (byte*)am_->getDbSetRange(rhsval);
			else
				page = (byte*)am_->getDbNext();
			break;
		case Predicate::OP_EQUAL:
			page = (byte*)am_->getDbSet(rhsval);
			done = true;
			break;
		case Predicate::OP_LESS_THAN:
			page = (byte*)am_->getDbNextRange(rhsval, m_bFirstCall);
			break;
		case Predicate::OP_LESS_THAN_OR_EQUAL:
			page = (byte*)am_->getDbNextRange(temp, m_bFirstCall);
			break;
		default:
			break;
		}

		if (page == NULL){
			done = true;
			break;
		}

		//02. Decode the page
		posdecoder->setBuffer(page);

		//03. Get the blocks
		MultiBlock* currBlockPos = (MultiBlock*)posdecoder->getNextBlock();

		//04. Check current position block to escape the loop
		if (currBlockPos == NULL){
			done = true;
			break;
		}

		//05. get the pairs one by one
		while (currBlockPos->hasNext()){
			pair = currBlockPos->getNext();
			m_pMatchedPredPos->addPosition(*(int*)pair->value);
		}
		m_bFirstCall = false;
	}

	delete posdecoder;

	//06. Check whether matchedPredPos is empty or not
	if (m_pMatchedPredPos->isNullSet())return false;
	else return true;
}
