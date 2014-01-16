#include "stdafx.h"
#include "BlockPrinter.h"
#include <iostream>
#include "ParallelHelper.h"
#include <omp.h>
#include <process.h>

//// There are three ways to calc the data from the datasource.
//	1 -- Single Thread
//  2 -- MultiThread
//  3 -- OpenMP
int BlockPrinter::MULTITHREAD_WAY = 1;

using namespace std;
BlockPrinter::BlockPrinter(ostream *_destStream, DataSource* dataSrc_[], int numCols_[], int numSrcs_)
{
	dataSrc = dataSrc_;
	numCols = numCols_;
	numSrcs = numSrcs_;
	nprinted = 0;
	blks = NULL;
	
	this->destStream = _destStream;

	stdOut = false;
}
BlockPrinter::BlockPrinter(DataSource* dataSrc_[], int numCols_[], int numSrcs_, char* fileName_)
{
	using namespace std;
	dataSrc = dataSrc_;
	numCols = numCols_;
	numSrcs = numSrcs_;
	nprinted = 0;
	blks = NULL;

	if (fileName_ == NULL) {
		stdOut = true;
	}
	else {
		outstream.open(fileName_);
		outstream.precision(15);

		stdOut = false;
	}
}

BlockPrinter::~BlockPrinter()
{
	outstream.close();
}

void BlockPrinter::printColumns(bool skip_output) {
	wrapperOfPrintColumns(skip_output, false);
}

void BlockPrinter::printColumnsToStream() {
	wrapperOfPrintColumns(true, true);
}

void BlockPrinter::wrapperOfPrintColumns(bool bSkipOutput, bool bOutStream)
{
	using namespace std;
	int totalNumCols = 0;
	int currCol;
	bool success = true;
	//PosBlock* posB;
	//Operator* PosFilter;
	MultiPosFilterBlock* posFilter;

	for (int i = 0; i<numSrcs; i++) {
		totalNumCols += numCols[i];
	}
	blks = new Block*[totalNumCols];
	for (int i = 0; i<totalNumCols; i++)
		blks[i] = NULL;

	PosOperator* posOperator = new PosOperator();
	MultiPosFilterBlock** aryMPFB = new MultiPosFilterBlock*[numSrcs];

#ifdef DEBUG
	DWORD dwBeginTimeQueryDb = GetTickCount();
#endif
	if (MULTITHREAD_WAY == 2)
	{
		// Here uses the multi-thread to calc the result.
		HANDLE* pThreadHandles = new HANDLE[numSrcs];
		ParallelHelper::tParams* pParams = new ParallelHelper::tParams[numSrcs];

		for (size_t i = 0; i < numSrcs; i++)
		{
			pParams[i].pDataSrc = dataSrc[i];
			pParams[i].pResult = NULL;
			pThreadHandles[i] = (HANDLE)_beginthreadex(NULL, 0, &ParallelHelper::threadFunc_DataSrc, &pParams[i], 0, NULL);
		}

		DWORD dwRet = ::WaitForMultipleObjects(numSrcs, pThreadHandles, true, 5000);

		for (size_t i = 0; i < numSrcs; i++)
		{
			aryMPFB[i] = pParams[i].pResult;
			::CloseHandle(pThreadHandles[i]);
		}

		delete[] pThreadHandles;
		delete[] pParams;

		for (int i = 0; i<numSrcs; i++){
			//zklee: Currently only consider AND situtation
			if (i>0)posOperator->addWhereOp('&');
			posOperator->addPosBlock(aryMPFB[i]);
		}
	}
	else if (MULTITHREAD_WAY == 3)
	{
		// Here is the OpenMP
#pragma omp parallel for num_threads(ParallelHelper::GetThreadNumber(numSrcs))
		for (int i = 0; i < numSrcs; i++)
		{
			aryMPFB[i] = dataSrc[i]->getPosOnPred();
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
			posOperator->addPosBlock(dataSrc[i]->getPosOnPred());
		}
	}

	posOperator->finishWhereOp();
#ifdef DEBUG
	DWORD dwEndTimeQueryDb = GetTickCount();
#endif

	posFilter = posOperator->getPosFilter();

#ifdef DEBUG
	DWORD dwEndTimeCalcPos = GetTickCount();

	cout << "Query Db Time: " << (double)(dwEndTimeQueryDb - dwBeginTimeQueryDb)/1000 << "s" << endl;
	cout << "Calc Pos Time: " << (double)(dwEndTimeCalcPos - dwEndTimeQueryDb)/1000 << "s" << endl;
#endif
	cout << posFilter->getNumValuesR() << endl;
	//posFilter->printBlocks();
	for (int i = 0; i<numSrcs; i++)
		dataSrc[i]->setPositionFilter(posFilter);

	while (success) {
		currCol = 0;
		for (int i = 0; i<numSrcs; i++) {
			for (int j = 0; j < numCols[i]; j++) {
				success &= printEntryForColumn(dataSrc[i], j, currCol, currCol == totalNumCols - 1, bSkipOutput, bOutStream);
				currCol++;
			}
		}
		++nprinted;
	}
	delete[] aryMPFB;
	delete[] blks;
	delete posFilter;
	delete posOperator;
}


void BlockPrinter::printColumnsWithPosition() {
	for (int i = 0; i<numSrcs; i++) {
		printColumnsWithPositionForSrc(dataSrc[i], numCols[i]);
	}
}

bool BlockPrinter::printEntryForColumn(DataSource* dataSrc_, int currCol, int currTotalCol, bool isLast, bool skip_output, bool bOutStream) {
	using namespace std;
	if (dataSrc_ == NULL) return false;
	if (blks[currTotalCol] == NULL){
		blks[currTotalCol] = dataSrc_->getNextValBlock(currCol);
		if (blks[currTotalCol] == NULL)
			return false;
	}

	ValPos* pair = blks[currTotalCol]->getNext();
	// DSM disable printing
	if (!skip_output) {
		if (!bOutStream)
		{
			if (stdOut) {
				pair->printVal(&cout);
				if (isLast)
					cout << endl;
				else
					cout << ",";
			}
			else
			{
				pair->printVal(&outstream);
				if (isLast)
					outstream << endl;
				else
					outstream << ",";
			}		
		}
		else
		{
			pair->printVal(this->destStream);
			if (isLast)
				(*destStream) << endl;
			else
				(*destStream) << ",";
		}
	}
#ifdef USE_GTEST
	else
	{
		char* szBuf = new char[pair->getSize() + 1];
		memset(szBuf, 0, pair->getSize() + 1);
		switch (pair->type)
		{
		case ValPos::INTTYPE:
			sprintf(szBuf, "%d", *((int*)pair->value));
			break;
		case ValPos::DOUBLETYPE:
			sprintf(szBuf, "%f", *((double*)pair->value));
			break;
		case ValPos::FLOATTYPE:
			sprintf(szBuf, "%f", *((float*)pair->value));
			break;
		case ValPos::LONGTYPE:
			sprintf(szBuf, "%d", *((long*)pair->value));
			break;
		case ValPos::STRINGTYPE:
			strncpy(szBuf, (char*)pair->value, pair->getSize());
		}

		std::string value = szBuf;
		m_aryQueryResult.push_back(value);

		delete[] szBuf;
	}
#endif 

	//BUG FIX: hasNext might be false now, but true next time this is called since the data blks[currTotalCol] is pointing to can be updated. However, we want to force getNextValBlock to be called to update the pointer, so put NULL in blks[currTotalCol] if !hasNext

	if (!blks[currTotalCol]->hasNext())
		blks[currTotalCol] = NULL;
	return true;

}

void BlockPrinter::printColumnsForSrc(DataSource* dataSrc_, int numCols_) {
	using namespace std;

	if (dataSrc_ == NULL) return;
	//Block* blks[numCols_];
	vector<Block*> blks(numCols_);

	for (int i = 0; i<numCols_; i++) {
		blks[i] = dataSrc_->getNextValBlock(i);
	}

	while (blks[0] != NULL) {
		for (int j = 0; j<blks[0]->getSize(); j++) {
			for (int col = 0; col<numCols_ - 1; col++) {
				ValPos* pair = blks[col]->getNext();

				if (stdOut) {
					pair->printVal(&cout);
					cout << ",";
				}
				else
				{
					//int before = StopWatch::ticks();
					pair->printVal(&outstream);
					outstream << ",";
					//int after = StopWatch::ticks();
					//totalPrint+= (after-before)*StopWatch::secondsPerTick();
					//cout << (after-before ) << " -------> " << (after-before ) << " seconds per tick? " << StopWatch::secondsPerTick() << endl;				  
				}
			}
			ValPos* pair = blks[numCols_ - 1]->getNext();
			if (stdOut) {
				pair->printVal(&cout);
				cout << endl;
			}
			else
			{
				pair->printVal(&outstream);
				//int before = StopWatch::ticks();
				outstream << endl;
				//int after = StopWatch::ticks();

				//totalPrint+= (after-before)*StopWatch::secondsPerTick();
				//cout << (after-before ) << " -------> " << (after-before ) << endl;
			}
		}
		for (int i = 0; i<numCols_; i++) {
			blks[i] = dataSrc_->getNextValBlock(i);
		}
	}

	//cout << " Found " << totalPrint << " Seconds for printing itself " << endl << " VS COUNT " << count << endl;
}

void BlockPrinter::printColumnsWithPositionForSrc(DataSource* dataSrc_, int numCols_) {
	using namespace std;

	if (dataSrc_ == NULL) return;
	//Block* blks[numCols_];
	vector<Block*> blks(numCols_);

	for (int i = 0; i<numCols_; i++) {
		blks[i] = dataSrc_->getNextValBlock(i);
	}

	while (blks[0] != NULL) {
		for (int j = 0; j<blks[0]->getSize(); j++) {
			for (int col = 0; col<numCols_ - 1; col++) {
				ValPos* pair = blks[col]->getNext();
				if (stdOut) {
					cout << "(";
					pair->printVal(&cout);
					cout << "," << pair->position << ")" << "\t\t";
				}
				else
				{
					outstream << "(";
					pair->printVal(&outstream);
					cout << "," << pair->position << ")" << "\t\t";
				}
			}
			ValPos* pair = blks[numCols_ - 1]->getNext();
			if (stdOut) {
				cout << "(";
				pair->printVal(&cout);
				cout << "," << pair->position << ")" << "\t\t" << endl;
			}
			else
			{
				outstream << "(";
				pair->printVal(&outstream);
				cout << "," << pair->position << ")" << "\t\t" << endl;
			}
		}
		for (int i = 0; i<numCols_; i++) {
			blks[i] = dataSrc_->getNextValBlock(i);
		}
	}
}
