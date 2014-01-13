#include "PosOperator.h"

PosOperator::PosOperator(){
	//caculatedPosBlock = new MultiPosFilterBlock();  
	m_pWhereOp = NULL;
	Level = 0;
	//m_pCaculatedPosBlock = NULL;
	m_pTempPosBlock1 = NULL;
	m_pTempPosBlock2 = NULL;
	//m_pCacuPosBlock = NULL;
	intermOp = '\0';
	prevOp = '\0';
	addWhereOp('(');//Start   
}

PosOperator::~PosOperator(){
	//delete caculatedPosBlock;
	m_pCurrWhereOp = m_pWhereOp;
	whereOp* whereOpTemp;
	while (m_pCurrWhereOp != NULL){
		whereOpTemp = m_pCurrWhereOp;
		//delete whereOpTemp->posBlock;
		m_pCurrWhereOp = whereOpTemp->nextOp;
		delete whereOpTemp;
	}
}

bool PosOperator::addWhereOp(char op){
	whereOp* whereOpTemp;
	whereOp* whereOpNew;

	whereOpNew = (whereOp*)malloc(sizeof(whereOp));
	if (whereOpNew == NULL)return false;
	whereOpNew->op = op;
	whereOpNew->nextOp = NULL;

	if (m_pWhereOp == NULL){
		m_pWhereOp = whereOpNew;
		return true;
	}

	whereOpTemp = m_pWhereOp;
	while (whereOpTemp->nextOp != NULL){
		whereOpTemp = whereOpTemp->nextOp;
	}
	whereOpTemp->nextOp = whereOpNew;
	return true;
}

void PosOperator::finishWhereOp(){
	addWhereOp(')');
}

bool PosOperator::addPosBlock(MultiPosFilterBlock* posBlock){
	whereOp* whereOpTemp;
	whereOp* whereOpNew;

	whereOpNew = (whereOp*)malloc(sizeof(whereOp));
	if (whereOpNew == NULL)return false;
	whereOpNew->op = 'P';
	whereOpNew->posBlock = posBlock;
	whereOpNew->nextOp = NULL;

	if (m_pWhereOp == NULL){
		m_pWhereOp = whereOpNew;
		return true;
	}

	whereOpTemp = m_pWhereOp;
	while (whereOpTemp->nextOp != NULL){
		whereOpTemp = whereOpTemp->nextOp;
	}
	whereOpTemp->nextOp = whereOpNew;
	return true;
}

MultiPosFilterBlock*  PosOperator::getPosFilter(){
	if (m_pWhereOp == NULL)
		return NULL;
	MultiPosFilterBlock* intermPosBlock = NULL;
	m_pCurrWhereOp = m_pWhereOp;
	char op;
	//char prevOp = '\0';
	while (parseWhereOpreation(op, intermPosBlock, prevOp)){
		if (op == '&' && intermPosBlock != NULL && intermPosBlock->hasNext()){
			m_spCaculatedPosBlock.reset(doAndCaculate(m_spCaculatedPosBlock.get(), intermPosBlock));
			delete intermPosBlock;
			intermPosBlock = NULL;
		}
		else if (op == '|' && intermPosBlock != NULL && intermPosBlock->hasNext()){
			m_spCaculatedPosBlock.reset(doOrCaculate(m_spCaculatedPosBlock.get(), intermPosBlock));
			delete intermPosBlock;
			intermPosBlock = NULL;
		}
		else if (op == 0 && intermPosBlock != NULL && intermPosBlock->hasNext()){
			m_spCaculatedPosBlock.reset(intermPosBlock);
			intermPosBlock = NULL;
		}
	}
	return m_spCaculatedPosBlock.release();
}

bool PosOperator::parseWhereOpreation(char& op_, MultiPosFilterBlock* &intermPosBlock_, char& prevOp){

	if (m_pCurrWhereOp == NULL)return false;
	whereOp* tempWhereOp = m_pCurrWhereOp;

	op_ = '\0';

	switch (tempWhereOp->op){
	case '(':
		if (prevOp == 'P')return false; //Parse error! Change to exception later
		if (prevOp == '&' || prevOp == '|'){
			intermPosBlock_ = m_pTempPosBlock1;
			intermOp = prevOp;
		}
		Level++;
		break;
	case ')':
		if (prevOp == '(' || prevOp == '&' || prevOp == '|')return false; //Parse error!
		if (prevOp == 'P'){
			intermPosBlock_ = m_pTempPosBlock1;
			op_ = intermOp;
			m_pTempPosBlock1 = NULL;
		}
		else if (prevOp == ')'){
			intermPosBlock_ = NULL;
			op_ = '\0';
		}
		Level--;
		break;
	case '&':
		if (prevOp == '(' || prevOp == '&' || prevOp == '|')return false; //Parse error!
		break;
	case '|':
		if (prevOp == '(' || prevOp == '&' || prevOp == '|')return false; //Parse error!
		break;
	case 'P':
		if (prevOp == 'P' || prevOp == ')')return false; //Parse error
		if (prevOp == '(')m_pTempPosBlock1 = tempWhereOp->posBlock->clone();
		else if (prevOp == '&' || prevOp == '|'){
			if (m_pTempPosBlock1 == NULL){
				m_pTempPosBlock1 = tempWhereOp->posBlock->clone();
				intermOp = prevOp;
			}
			else if (m_pTempPosBlock2 == NULL){
				m_pTempPosBlock2 = tempWhereOp->posBlock->clone();
			}
		}

		if (m_pTempPosBlock1 != NULL && m_pTempPosBlock2 != NULL){
			if (prevOp == '&'){
				m_pTempPosBlock1 = doAndCaculate(m_pTempPosBlock1, m_pTempPosBlock2);
				m_pTempPosBlock2 = NULL;
			}
			else if (prevOp == '|'){
				m_pTempPosBlock1 = doOrCaculate(m_pTempPosBlock1, m_pTempPosBlock2);
				m_pTempPosBlock2 = NULL;
			}
			else{//Parse error!
				delete m_pTempPosBlock1;
				delete m_pTempPosBlock2;
				return false;
			}
		}
		break;
	default:
		break;
	}

	if (Level < 0 || Level >2)return false; //Parse error! Currently Only support 2 levels.
	prevOp = tempWhereOp->op;
	m_pCurrWhereOp = tempWhereOp->nextOp;
	return true;
}

MultiPosFilterBlock* PosOperator::doAndCaculate(MultiPosFilterBlock* posBlock1_, MultiPosFilterBlock* posBlock2_) const{
	//if(cacuPosBlock)delete cacuPosBlock;
	bool done = false;
	bool firstTime = true;

	PosFilterBlock* tempPosFilterBlock;
	PosFilterBlock* posFilterBlock1;
	PosFilterBlock* posFilterBlock2;
	PosFilterBlock* tempFilterBlock1;
	PosFilterBlock* tempFilterBlock2;
	unsigned int int1;
	unsigned int int2;
	unsigned int tempInt;

	if (posBlock1_->isCompleteSet()){
		//delete posBlock1_;
		return posBlock2_;
	}
	else if (posBlock2_->isCompleteSet()){
		//delete posBlock2_;
		return posBlock1_;
	}

	if (posBlock1_->isNullSet()){
		//delete posBlock2_;
		return posBlock1_;
	}
	else if (posBlock2_->isNullSet()){
		//delete posBlock1_;
		return posBlock2_;
	}

	auto_ptr<MultiPosFilterBlock> spCacuPosBlock(new MultiPosFilterBlock());
	posFilterBlock1 = posBlock1_->getFirstBlock();
	posFilterBlock2 = posBlock2_->getFirstBlock();

	while (posFilterBlock1 != NULL && posFilterBlock2 != NULL){

		if (posFilterBlock1->getCurrIntStartPos() > posFilterBlock2->getEndPosition()){
			posFilterBlock2 = posBlock2_->getNextBlock();
			continue;
		}

		if (posFilterBlock2->getCurrIntStartPos() > posFilterBlock1->getEndPosition()){
			posFilterBlock1 = posBlock1_->getNextBlock();
			continue;
		}

		done = false;
		if (posFilterBlock1->getCurrIntStartPos() >= posFilterBlock2->getCurrIntStartPos()){
			tempFilterBlock1 = posFilterBlock1;
			tempFilterBlock2 = posFilterBlock2;
		}
		else if (posFilterBlock1->getCurrIntStartPos() < posFilterBlock2->getCurrIntStartPos()){
			tempFilterBlock1 = posFilterBlock2;
			tempFilterBlock2 = posFilterBlock1;
		}

		int1 = tempFilterBlock1->getCurrIntValue();
		tempFilterBlock2->cutGetRightAtPos(tempFilterBlock1->getCurrIntStartPos());
		int2 = tempFilterBlock2->getCurrIntValue();

		tempPosFilterBlock = new PosFilterBlock();
		tempPosFilterBlock->initEmptyBuffer(tempFilterBlock1->getStartPosition());
		tempPosFilterBlock->setCurrInt(0);
		while (!done){
			tempInt = int1 & int2;
			if (!tempPosFilterBlock->addInt(tempInt)){//if Block is full,create a new one
				spCacuPosBlock->addPosFilterBlock(tempPosFilterBlock);
				tempPosFilterBlock = new PosFilterBlock();
				tempPosFilterBlock->initEmptyBuffer(tempFilterBlock1->getCurrIntRealStartPos());
				tempPosFilterBlock->setCurrInt(0);
				tempPosFilterBlock->addInt(tempInt);//Aways true
			};
			if (tempFilterBlock1->hasNextInt() && tempFilterBlock2->hasNextInt()){
				int1 = tempFilterBlock1->getNextInt();
				int2 = tempFilterBlock2->getNextInt();
			}
			else{
				if (!tempFilterBlock1->hasNextInt() && tempFilterBlock2->hasNext()){
					posFilterBlock1 = ((MultiPosFilterBlock*)(tempFilterBlock1->parentMultiBlock))->getNextBlock();
					posFilterBlock2 = tempFilterBlock2;
				}
				else if (!tempFilterBlock2->hasNextInt() && tempFilterBlock1->hasNext()){
					posFilterBlock1 = ((MultiPosFilterBlock*)(tempFilterBlock2->parentMultiBlock))->getNextBlock();
					posFilterBlock2 = tempFilterBlock1;
				}
				else{
					posFilterBlock1 = posBlock1_->getNextBlock();
					posFilterBlock2 = posBlock2_->getNextBlock();
				}
				spCacuPosBlock->addPosFilterBlock(tempPosFilterBlock);
				done = true;
			}

		}
	}

	//delete posBlock1_;
	//delete posBlock2_;
	spCacuPosBlock->optimize();
	spCacuPosBlock->setCurrBlock(0);
	return spCacuPosBlock.release();
}

MultiPosFilterBlock* PosOperator::doOrCaculate(MultiPosFilterBlock* posBlock1_, MultiPosFilterBlock* posBlock2_) const{
	//if(cacuPosBlock)delete cacuPosBlock;
	bool done = false;


	//if(ifPrint)posBlock1_->printBlocks();

	PosFilterBlock* tempPosFilterBlock;
	PosFilterBlock* posFilterBlock1;
	PosFilterBlock* posFilterBlock2;
	PosFilterBlock* tempFilterBlock1;
	PosFilterBlock* tempFilterBlock2;
	unsigned int int1;
	unsigned int int2;
	unsigned int tempInt;

	if (posBlock1_->isCompleteSet()){
		//delete posBlock2_;
		return posBlock1_;
	}
	else if (posBlock2_->isCompleteSet()){
		//delete posBlock1_;
		return posBlock2_;
	}

	if (posBlock1_->isNullSet()){
		//delete posBlock1_;
		return posBlock2_;
	}
	else if (posBlock2_->isNullSet()){
		//delete posBlock2_;
		return posBlock1_;
	}

	auto_ptr<MultiPosFilterBlock> spCacuPosBlock(new MultiPosFilterBlock);
	posFilterBlock1 = posBlock1_->getFirstBlock();
	posFilterBlock2 = posBlock2_->getFirstBlock();
	//bool ifPrint;
	while (true){

		if (posFilterBlock1 == NULL && posFilterBlock2 != NULL){
			spCacuPosBlock->addPosFilterBlock(posFilterBlock2->cutGetRightAtPos(posFilterBlock2->getCurrIntStartPos())->clone());
			posFilterBlock2 = ((MultiPosFilterBlock*)(posFilterBlock2->parentMultiBlock))->getNextBlock();
			continue;
		}
		else if (posFilterBlock1 != NULL && posFilterBlock2 == NULL){
			spCacuPosBlock->addPosFilterBlock(posFilterBlock1->cutGetRightAtPos(posFilterBlock1->getCurrIntStartPos())->clone());
			posFilterBlock1 = ((MultiPosFilterBlock*)(posFilterBlock1->parentMultiBlock))->getNextBlock();
			continue;
		}
		else if (posFilterBlock1 == NULL && posFilterBlock2 == NULL)//run out of the blocks
			break;

		/*if(posFilterBlock2->getStartPosition() == 766584){
		ifPrint = true;
		}*/

		if (posFilterBlock1->getCurrIntStartPos() > posFilterBlock2->getEndPosition()){
			spCacuPosBlock->addPosFilterBlock(posFilterBlock2->cutGetRightAtPos(posFilterBlock2->getCurrIntStartPos())->clone());
			posFilterBlock2 = ((MultiPosFilterBlock*)(posFilterBlock2->parentMultiBlock))->getNextBlock();
			continue;
		}

		if (posFilterBlock2->getCurrIntStartPos() > posFilterBlock1->getEndPosition()){
			spCacuPosBlock->addPosFilterBlock(posFilterBlock1->cutGetRightAtPos(posFilterBlock1->getCurrIntStartPos())->clone());
			posFilterBlock1 = ((MultiPosFilterBlock*)(posFilterBlock1->parentMultiBlock))->getNextBlock();
			continue;
		}

		done = false;
		if (posFilterBlock1->getCurrIntStartPos() == posFilterBlock2->getCurrIntStartPos()){
			tempFilterBlock1 = posFilterBlock1;
			tempFilterBlock2 = posFilterBlock2;
			int1 = tempFilterBlock1->getCurrIntValue();
			int2 = tempFilterBlock2->getCurrIntValue();
		}
		else{
			if (posFilterBlock1->getCurrIntStartPos() > posFilterBlock2->getCurrIntStartPos()){
				tempFilterBlock1 = posFilterBlock1;
				tempFilterBlock2 = posFilterBlock2;
			}
			else if (posFilterBlock1->getCurrIntStartPos() < posFilterBlock2->getCurrIntStartPos()){
				tempFilterBlock1 = posFilterBlock2;
				tempFilterBlock2 = posFilterBlock1;
			}
			spCacuPosBlock->addPosFilterBlock(tempFilterBlock2->clone()->cutGetLeftAtPos((tempFilterBlock1->getCurrIntStartPos() - 1)));
			int1 = tempFilterBlock1->getCurrIntValue();
			tempFilterBlock2->cutGetRightAtPos(tempFilterBlock1->getCurrIntStartPos());
			int2 = tempFilterBlock2->getCurrIntValue();
		}

		tempPosFilterBlock = new PosFilterBlock();
		tempPosFilterBlock->initEmptyBuffer(tempFilterBlock1->getStartPosition());
		tempPosFilterBlock->setCurrInt(0);

		while (!done){
			tempInt = int1 | int2;
			if (!tempPosFilterBlock->addInt(tempInt)){//if Block is full,create a new one
				spCacuPosBlock->addPosFilterBlock(tempPosFilterBlock);
				tempPosFilterBlock = new PosFilterBlock();
				tempPosFilterBlock->initEmptyBuffer(tempFilterBlock1->getCurrIntRealStartPos());
				tempPosFilterBlock->setCurrInt(0);
				tempPosFilterBlock->addInt(tempInt);//Always true
			};
			if (tempFilterBlock1->hasNextInt() && tempFilterBlock2->hasNextInt()){
				int1 = tempFilterBlock1->getNextInt();
				int2 = tempFilterBlock2->getNextInt();
			}
			else{
				if (!tempFilterBlock1->hasNextInt() && tempFilterBlock2->hasNextInt()){
					tempPosFilterBlock->cutGetLeftAtPos(tempFilterBlock1->getEndPosition());
					posFilterBlock2 = tempFilterBlock2->cutGetRightAtPos(tempFilterBlock1->getEndPosition() + 1);
					posFilterBlock1 = ((MultiPosFilterBlock*)(tempFilterBlock1->parentMultiBlock))->getNextBlock();
				}
				else if (!tempFilterBlock2->hasNextInt() && tempFilterBlock1->hasNextInt()){
					tempPosFilterBlock->cutGetLeftAtPos(tempFilterBlock2->getEndPosition());
					posFilterBlock2 = tempFilterBlock1->cutGetRightAtPos(tempFilterBlock2->getEndPosition() + 1);
					posFilterBlock1 = ((MultiPosFilterBlock*)(tempFilterBlock2->parentMultiBlock))->getNextBlock();
				}
				else{
					posFilterBlock1 = posBlock1_->getNextBlock();
					posFilterBlock2 = posBlock2_->getNextBlock();
				}
				spCacuPosBlock->addPosFilterBlock(tempPosFilterBlock);
				done = true;
			}

		}
	}

	//delete posBlock1_;
	//delete posBlock2_;
	spCacuPosBlock->optimize();
	spCacuPosBlock->setCurrBlock(0);
	//cacuPosBlock->getNumValuesR();
	//if(ifPrint)cacuPosBlock->printBlocks();
	return spCacuPosBlock.release();
}