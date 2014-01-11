#include "DataSource.h"
#include "StringUtil.h"

using namespace std;

DataSource::DataSource(AM* am_, bool isROS_) {
	init(am_, isROS_);
}

DataSource::DataSource(AM* am_, bool isROS_, bool valSorted_, Decoder* decoder_) {
	init(am_, isROS_);
	valSorted = valSorted_;
	m_spDecoder.reset(decoder_);
	currBlock = NULL;

}

void DataSource::init(AM* am_, bool isROS_) {
	if (am_ == NULL)
		throw new UnexpectedException("DataSource::constructor Error, am is NULL");
	am = am_;

	if (!isROS_)
		posPrimaryIndex = false;
	else {
		posPrimaryIndex = true;
	}
	isROS = isROS_;

	am->initCursors();
	posFilterChanged = true;
	predChanged = true;
	m_pPred = NULL;

	posFilter = NULL;
	filterCursor = NULL;
	matchedPredPos = NULL;
	posOutTripleOnPred = new RLETriple();
	firstCall = true;
}

DataSource::~DataSource() {
	if (posOutTripleOnPred){
		delete posOutTripleOnPred;
		posOutTripleOnPred = NULL;
	}
	if (matchedPredPos){
		delete matchedPredPos;
		matchedPredPos = NULL;
	}
	if (filterCursor){
		delete filterCursor;
		filterCursor = NULL;
	}
}

// Changes the RHS binding for this datasource
void DataSource::changeRHSBinding(ValPos* rhs_) {
	if (m_pPred == NULL)
		throw new UnexpectedException("DataSource: Error, setting binding on NULL predicate");
	else {
		m_pPred->setRHS(rhs_);
		predChanged = true;
	}
}

// sets a new predicate for this datasource
void DataSource::setPredicate(Predicate* pred_) {
	predChanged = true;
	m_pPred = pred_;
}

// Sets a filter for postions
void DataSource::setPositionFilter(MultiPosFilterBlock* bitstringDataSource_) {
	posFilterChanged = true;
	posFilter = bitstringDataSource_;
}


//getLastPosition returns the last position in the data source (column)
int DataSource::getLastPosition() {
	if (posPrimaryIndex)
		return *(int*)am->getLastIndexValuePrimary();
	else
		return 1; //To be implemented
}

const void* DataSource::getNextPageValue() {
	if (posPrimaryIndex)
		return am->getNextPagePrimary();
	else
		return NULL;//To be implemented
}

const void* DataSource::skipToPageValue(char* key) {
	if (posPrimaryIndex)
		return am->skipToPageSecondary(key);
	else
		return am->skipToPagePrimary(key);
}

const void* DataSource::skipToPagePosition(int key) {
	if (posPrimaryIndex)
		return am->skipToPagePrimary((char*)&key);
	else
		return NULL;//To be implemented
}

Block* DataSource::getDecodedBlock() {
	return m_spDecoder->getNextBlock();
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
	m_spDecoder->setBuffer(page);
	if (posFilter != NULL){
		unsigned int currStartPos = filterCursor->getCurrStartPosition();
		skipToRightPosOnPage(currStartPos);
		currBlock = (MultiBlock*)getDecodedBlock();
		if (currBlock == NULL) return NULL;
		currBlock->filterWithPos(filterCursor);
	}
	else{
		currBlock = (MultiBlock*)getDecodedBlock();
	}

	return currBlock;
}

byte* DataSource::getRightPage() {
	Log::writeToLog("DataSource", 0, "Called GetRightPage()");
	// if anything has changed we need to reset the indices
	if (posFilterChanged || predChanged) {
		Log::writeToLog("Datasource", 0, "Filter or pred changed, initing cursors");
		am->initCursors();
		posFilterChanged = false;
		predChanged = false;
	}

	if (posFilter != NULL){
		if (posFilter->isCompleteSet()){
			posFilter = NULL;
			return getPage();//Sequential Reading
		}
		else if (posFilter->isNullSet())
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
	if (filterCursor == NULL){
		filterCursor = posFilter->getCursor();
		filterCursor->getNext();//Move to the start position
		filterCursor->setCurrStartPosition(); //Record current start position
	}

	if (!filterCursor->isFilterFinished()){
		am->initCursors();
		return (byte*)skipToPagePosition(filterCursor->getCurrStartPosition());
	}
	else return NULL;
}

// Finds the right block on the page
bool DataSource::skipToRightPosOnPage(unsigned int pos_) {
	if (m_spDecoder->skipToPos(pos_))
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
	matchedPredPos = new MultiPosFilterBlock();
	if (m_pPred == NULL)matchedPredPos->setCompleteSet(true);
	else{
		predChanged = false;//Reset predChanged

		ValPos* rhsvp = m_pPred->getRHS();
		char* rhsval = (char*)rhsvp->value;
		ValPos* tempVP = rhsvp->clone();
		int valsize = m_pPred->getRHS()->getSize();
		unsigned char* temp;
		if (m_pPred->getRHS()->type == ValPos::STRINGTYPE){
			temp = StringUtil::getSmallestLargerValue(rhsval, valsize);
			tempVP->set(temp);
		}
		else{//Default is string type column.
			return NULL;
		}

		if (valSorted) {
			getPosOnPredValueSorted(rhsvp, tempVP);
		}
		else {// The column is not value sorted
			getPosOnPredValueUnsorted((ROSAM*)am, rhsvp, tempVP);
		}
	}
	return matchedPredPos;
}
//Get postion on predition if the value is sorted:Check
bool DataSource::getPosOnPredValueSorted(ValPos* rhsvp_, ValPos* tempVP_){

	if (!posOutTripleOnPred->isNull()) return false;
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
			m_spDecoder->setBuffer(page);
			if (m_spDecoder->skipToBlockOnValue(tempVP_))
				position = ((MultiBlock*)getDecodedBlock(m_spDecoder.get()))->getPosition();
		}
		else return false;
		if (position == 0) posOutTripleOnPred->setTriple(NULL, minPos, maxPos - minPos + 1);
		if (position == maxPos)return false;
		else
			posOutTripleOnPred->setTriple(NULL, position, maxPos - position + 1);
		break;
	case Predicate::OP_GREATER_THAN_OR_EQUAL:
		page = (byte*)skipToPageValue(rhsval);
		if (page != NULL) {
			m_spDecoder->setBuffer(page);
			if (m_spDecoder->skipToBlockOnValue(rhsvp_))
				position = ((MultiBlock*)getDecodedBlock(m_spDecoder.get()))->getPosition();
		}
		if (position == 0) matchedPredPos->setCompleteSet(true);
		if (position == maxPos)return false;
		else posOutTripleOnPred->setTriple(NULL, position, maxPos - position + 1);
		break;
	case Predicate::OP_EQUAL:
		page = (byte*)skipToPageValue(rhsval);
		if (page != NULL) {
			m_spDecoder->setBuffer(page);
			if (m_spDecoder->skipToBlockOnValue(rhsvp_))
				position = ((MultiBlock*)getDecodedBlock(m_spDecoder.get()))->getPosition();
		}
		if (position == 0)return false;
		unsigned int end;
		page = (byte*)skipToPageValue(temp);
		if (page != NULL) {
			m_spDecoder->setBuffer(page);
			if (m_spDecoder->skipToBlockOnValue(tempVP_))
				end = ((MultiBlock*)getDecodedBlock(m_spDecoder.get()))->getPosition() - 1;
		}
		else end = 0;

		if (end == 0){
			if (position == 1)matchedPredPos->setCompleteSet(true);
			else posOutTripleOnPred->setTriple(NULL, position, maxPos - position + 1);
		}
		else posOutTripleOnPred->setTriple(NULL, position, end - position + 1);
		break;
	case Predicate::OP_LESS_THAN:
		//if (position==minPos) return false;
		page = (byte*)skipToPageValue(rhsval);
		if (page != NULL) {
			m_spDecoder->setBuffer(page);
			if (m_spDecoder->skipToBlockOnValue(rhsvp_))
				position = ((MultiBlock*)getDecodedBlock(m_spDecoder.get()))->getPosition();
		}
		else matchedPredPos->setCompleteSet(true);
		position--;
		if (position >= 1 && !matchedPredPos->isCompleteSet())
			posOutTripleOnPred->setTriple(NULL, minPos, position - minPos + 1);
		break;
	case Predicate::OP_LESS_THAN_OR_EQUAL:
		//if (position==minPos)return false;
		page = (byte*)skipToPageValue(temp);
		if (page != NULL) {
			m_spDecoder->setBuffer(page);
			if (m_spDecoder->skipToBlockOnValue(tempVP_))
				position = ((MultiBlock*)getDecodedBlock(m_spDecoder.get()))->getPosition();
		}
		else matchedPredPos->setCompleteSet(true);
		position--;
		if (position >= 1 && !matchedPredPos->isCompleteSet())
			posOutTripleOnPred->setTriple(NULL, minPos, position - minPos + 1);
		break;
	default:
		return false;
		break;
	}

	if (!matchedPredPos->isCompleteSet())
		matchedPredPos->setTriple(posOutTripleOnPred);
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
			if (firstCall)
				page = (byte*)am_->getDbSetRange(temp);
			else
				page = (byte*)am_->getDbNext();
			break;
		case Predicate::OP_GREATER_THAN_OR_EQUAL:
			if (firstCall)
				page = (byte*)am_->getDbSetRange(rhsval);
			else
				page = (byte*)am_->getDbNext();
			break;
		case Predicate::OP_EQUAL:
			page = (byte*)am_->getDbSet(rhsval);
			done = true;
			break;
		case Predicate::OP_LESS_THAN:
			page = (byte*)am_->getDbNextRange(rhsval, firstCall);
			break;
		case Predicate::OP_LESS_THAN_OR_EQUAL:
			page = (byte*)am_->getDbNextRange(temp, firstCall);
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
			matchedPredPos->addPosition(*(int*)pair->value);
		}
		firstCall = false;
	}

	delete posdecoder;

	//06. Check whether matchedPredPos is empty or not
	if (matchedPredPos->isNullSet())return false;
	else return true;
}
