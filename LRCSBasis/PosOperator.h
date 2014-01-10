#pragma once

#ifdef LRCSBASIS_EXPORTS
#define LRCSBasis_API __declspec(dllexport)
#else
#define LRCSBasis_API __declspec(dllimport)
#endif

#include "MultiPosFilterBlock.h"
#include "assert.h"
#include <stdlib.h>
#include <memory>

struct  whereOp
{
	char op; //'(' = '(', 'AND' = '&', 'OR' = '|',')' = ')', 'Predication' = 'P'
	MultiPosFilterBlock* posBlock;
	whereOp* nextOp;
};

class LRCSBasis_API PosOperator
{
public:
	PosOperator();
	virtual ~PosOperator();

	// Add where operators to be caculated
	virtual	bool addWhereOp(char op);

	virtual void finishWhereOp();

	// Add position filter blocks
	virtual	bool addPosBlock(MultiPosFilterBlock* posBlock);

	// Parse the where conditions and do "And" "Or" Caculate to get the final position filters
	virtual	MultiPosFilterBlock* getPosFilter();

protected:

	whereOp* m_pWhereOp;
	whereOp* m_pCurrWhereOp;
	auto_ptr<MultiPosFilterBlock> m_spCaculatedPosBlock;
	MultiPosFilterBlock* m_pTempPosBlock1;
	MultiPosFilterBlock* m_pTempPosBlock2;
	//MultiPosFilterBlock* m_pCacuPosBlock;
	//char prevOp;
	char intermOp;
	int Level;

	virtual bool parseWhereOpreation(char& op_, MultiPosFilterBlock* &intermPosBlock_, char& prevOp);
	virtual MultiPosFilterBlock* doAndCaculate(MultiPosFilterBlock* posBlock1_, MultiPosFilterBlock* posBlock2_) const;
	virtual MultiPosFilterBlock* doOrCaculate(MultiPosFilterBlock* posBlock1_, MultiPosFilterBlock* posBlock2_) const;
};

