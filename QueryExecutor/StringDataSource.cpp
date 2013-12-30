#include "StringDataSource.h"
#include "StringUtil.h"
#include "StringDecoder.h"

StringDataSource::StringDataSource(AM* am_, bool valSorted_, bool isROS)
: DataSource(am_, isROS)
{
	m_bValSorted = valSorted_;
	m_pDecoder = new StringDecoder(m_bValSorted);
}


StringDataSource::~StringDataSource()
{

	if (m_pDecoder != NULL) {
		delete m_pDecoder;
		m_pDecoder = NULL;
	}

	//if (currBlock!=NULL) delete currBlock;
}

//Get the position block on predicaiton
MultiPosFilterBlock* StringDataSource::getPosOnPred(){
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
		temp = StringUtil::getSmallestLargerValue(rhsval, valsize);
		tempVP->set(temp);

		if (m_bValSorted) {
			getPosOnPredValueSorted(rhsvp, tempVP);
		}
		else {// The column is not value sorted
			getPosOnPredValueUnsorted((ROSAM*)m_pAM, rhsvp, tempVP);
		}
	}
	return m_pMatchedPredPos;
}

//printColumn, print to stringstream instead. For testing purpose
void StringDataSource::printColumn() {
	using namespace std;
	int count = 0;

	Block* blk = this->getNextValBlock(1);
	while (blk != NULL) {
		while (blk->hasNext()) {
			ValPos* vp = blk->getNext();
			cout << (char*)vp->value << endl;
		}
		blk = this->getNextValBlock(0);
	}
}//printColumn, print to stringstream instead. For testing purpose
