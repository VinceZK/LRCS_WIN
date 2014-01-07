#pragma once
#include "Operator.h"
#include "Block.h"
#include "StopWatch.h"
#include <iostream>
#include <fstream>
#include "MultiPosFilterBlock.h"
#include "DataSource.h"
#include "PosOperator.h"

#ifdef QUERYEXECUTOR_EXPORTS
#define QueryExecutor_API	__declspec(dllexport)
#else
#define QueryExecutor_API	__declspec(dllimport)
#endif


class QueryExecutor_API BlockPrinter
{
public:
	BlockPrinter(DataSource* dataSrc_[], int numCols_[], int numSrcs_, char* fileName_);
	virtual ~BlockPrinter();
	void printColumns(bool skip_output = false);
	void printColumnsWithPosition();
	int getNumPrinted() { return nprinted; }
protected:
	void printColumnsForSrc(DataSource* dataSrc_, int numCols_);
	bool printEntryForColumn(DataSource* dataSrc_, int currCol, int currTotalCol, bool isLast, bool skip_output);
	void printColumnsWithPositionForSrc(DataSource* dataSrc_, int numCols_);
	DataSource** dataSrc;
	std::ofstream outstream;
	int* numCols;
	bool stdOut;
	int numSrcs;
	int nprinted;
	Block** blks;
	int totalCols;
};

