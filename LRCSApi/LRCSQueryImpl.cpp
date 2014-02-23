#include "stdafx.h"
#include "LRCSQueryImpl.h"
#include "DbHandler.h"
#include "../LRCSBasis/MultiPosFilterBlock.h"
#include "../LRCSBasis/PosOperator.h"
#include "../LRCSBasis/DataSource.h"

int LRCSQueryImpl::MULTITHREAD_WAY = 3;

LRCSQueryImpl::LRCSQueryImpl()
{
}


LRCSQueryImpl::~LRCSQueryImpl()
{
}

QueryResult	LRCSQueryImpl::DoQuery(DbHandler* pDbHandler)
{
	QueryResult result;
	using namespace std;
	int totalNumCols = 0;
	int currCol;
	bool success = true;

	MultiPosFilterBlock* posFilter;

	std::vector<DbHandler::DataSourceInfo> allDataSources = pDbHandler->GetDataSources();
	int numSrcs = allDataSources.size();

	PosOperator* posOperator = new PosOperator();
	MultiPosFilterBlock** aryMPFB = new MultiPosFilterBlock*[numSrcs];

#ifdef DEBUG
	DWORD dwBeginTimeQueryDb = GetTickCount();
#endif
	//if (MULTITHREAD_WAY == 2)
	//{
	//	// Here uses the multi-thread to calc the result.
	//	HANDLE* pThreadHandles = new HANDLE[numSrcs];
	//	ParallelHelper::tParams* pParams = new ParallelHelper::tParams[numSrcs];

	//	for (size_t i = 0; i < numSrcs; i++)
	//	{
	//		pParams[i].pDataSrc = dataSrc[i];
	//		pParams[i].pResult = NULL;
	//		pThreadHandles[i] = (HANDLE)_beginthreadex(NULL, 0, &ParallelHelper::threadFunc_DataSrc, &pParams[i], 0, NULL);
	//	}

	//	DWORD dwRet = ::WaitForMultipleObjects(numSrcs, pThreadHandles, true, 5000);

	//	for (size_t i = 0; i < numSrcs; i++)
	//	{
	//		aryMPFB[i] = pParams[i].pResult;
	//		::CloseHandle(pThreadHandles[i]);
	//	}

	//	delete[] pThreadHandles;
	//	delete[] pParams;

	//	for (int i = 0; i<numSrcs; i++){
	//		//zklee: Currently only consider AND situtation
	//		if (i>0)posOperator->addWhereOp('&');
	//		posOperator->addPosBlock(aryMPFB[i]);
	//	}
	//}
//		else 
	if (MULTITHREAD_WAY == 3)
	{
		// Here is the OpenMP
#pragma omp parallel for num_threads(ParallelHelper::GetThreadNumber(numSrcs))
		for (int i = 0; i < numSrcs; i++)
		{
			aryMPFB[i] = allDataSources[i].first->getPosOnPred();
		}

		for (int i = 0; i<numSrcs; i++){
			//zklee: Currently only consider AND situtation
			if (i>0)posOperator->addWhereOp('&');
			posOperator->addPosBlock(aryMPFB[i]);
		}
	}
	else
	{// Here is single thread.
		for (int i = 0; i<numSrcs; i++){
			//zklee: Currently only consider AND situtation
			if (i>0)posOperator->addWhereOp('&');
			posOperator->addPosBlock(allDataSources[i].first->getPosOnPred());
		}
	}

	posOperator->finishWhereOp();
#ifdef DEBUG
	DWORD dwEndTimeQueryDb = GetTickCount();
#endif

	posFilter = posOperator->getPosFilter();

#ifdef DEBUG
	DWORD dwEndTimeCalcPos = GetTickCount();

	cout << "Query Db Time: " << (double)(dwEndTimeQueryDb - dwBeginTimeQueryDb) / 1000 << "s" << endl;
	cout << "Calc Pos Time: " << (double)(dwEndTimeCalcPos - dwEndTimeQueryDb) / 1000 << "s" << endl;
#endif
	cout << posFilter->getNumValuesR() << endl;
	//posFilter->printBlocks();
	for (int i = 0; i<numSrcs; i++)
		allDataSources[i].first->setPositionFilter(posFilter);

	int nTotalColumn = pDbHandler->TotalColumn();
	Block** pBlocks = new Block*[nTotalColumn];
	for (int i = 0; i < nTotalColumn; ++i)
	{
		pBlocks[i] = NULL;
	}

	do{
		QueryResult::QueryResultRow row;
		int currCol = 0;

		for (int i = 0; i < allDataSources.size(); ++i)
		{
			for (int j = 0; j < allDataSources[i].second.second; ++j)
			{
				LRCSValue value;
				success &= FillDataToQueryResult(allDataSources[i].first, j, currCol, pBlocks, value);
				if (success)
					row.push_back(value);
				++currCol;
			}
		}

		if (row.size() > 0)
			result.AddRow(row);
	} while (success);

	delete[] aryMPFB;
	delete[] pBlocks;
	delete posFilter;
	delete posOperator;

	return result;
}

bool LRCSQueryImpl::FillDataToQueryResult(DataSource* dataSrc_, int currCol, int currTotalCol, Block** pBlks, LRCSValue& value) {
	using namespace std;
	if (dataSrc_ == NULL) 
		return false;

	if (pBlks[currTotalCol] == NULL)
	{
		pBlks[currTotalCol] = dataSrc_->getNextValBlock(currCol);
		if (pBlks[currTotalCol] == NULL)
			return false;	
	}

	ValPos* pair = pBlks[currTotalCol]->getNext();
	// DSM disable printing

	value.SetValue(pair->value, pair->getSize(), (LRCSValue::LRCSValueType)pair->type);

	//BUG FIX: hasNext might be false now, but true next time this is called since the data blks[currTotalCol] is pointing to can be updated. However, we want to force getNextValBlock to be called to update the pointer, so put NULL in blks[currTotalCol] if !hasNext

	if (!pBlks[currTotalCol]->hasNext())
		pBlks[currTotalCol] = NULL;
	return true;

}