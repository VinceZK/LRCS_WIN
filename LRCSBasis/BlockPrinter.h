#pragma once
#include "Operator.h"
#include "Block.h"
#include "StopWatch.h"
#include <iostream>
#include <fstream>
#include "MultiPosFilterBlock.h"
#include "DataSource.h"
#include "PosOperator.h"

#ifdef USE_GTEST
#include <gtest\gtest.h>
#endif

using namespace std;

class LRCSBasis_API BlockPrinter
{
public:	// Unit Test by gtest
#ifdef USE_GTEST
	FRIEND_TEST(TestBlockPrinter, TestMethod_PrintColumns);

	std::vector<std::string>	m_aryQueryResult;
#endif

public:
	BlockPrinter(ostream *destStream, DataSource* dataSrc_[], int numCols_[], int numSrcs_);
	BlockPrinter(DataSource* dataSrc_[], int numCols_[], int numSrcs_, char* fileName_);
	virtual ~BlockPrinter();
	void printColumns(bool skip_output = false);
	void printColumnsToStream();
	void printColumnsWithPosition();
	int getNumPrinted() { return nprinted; }
protected:
	/// Encapsulat the logic of the printColumns() and the printColumnsToStream()
	/// since the most logic are same.
	void wrapperOfPrintColumns(bool bSkipOutput, bool bOutStream);

	void printColumnsForSrc(DataSource* dataSrc_, int numCols_);
	bool printEntryForColumn(DataSource* dataSrc_, int currCol, int currTotalCol, bool isLast, bool skip_output, bool bOutStream = false);
	//bool printEntryForColumnToStream(DataSource* dataSrc_, int currCol, int currTotalCol, bool isLast);
	void printColumnsWithPositionForSrc(DataSource* dataSrc_, int numCols_);
	DataSource** dataSrc;
	std::ofstream outstream;
	int* numCols;
	bool stdOut;
	int numSrcs;
	int nprinted;
	Block** blks;
	int totalCols;
	ostream *destStream;

private:
	static int MULTITHREAD_WAY;
};

