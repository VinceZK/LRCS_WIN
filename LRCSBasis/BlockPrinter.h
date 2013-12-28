#pragma once
#include "Operator.h"
#include "Block.h"
#include "StopWatch.h"
#include <iostream>
#include <fstream>
#include "MultiPosFilterBlock.h"
#include "DataSource.h"
#include "PosOperator.h"

using namespace std;

class __declspec(dllexport) BlockPrinter
{
public:
	BlockPrinter(ostream *destStream, DataSource* dataSrc_[], int numCols_[], int numSrcs_);
	BlockPrinter(DataSource* dataSrc_[], int numCols_[], int numSrcs_, char* fileName_);
	virtual ~BlockPrinter();
	void printColumns(bool skip_output = false);
	void printColumnsToStream();
	void printColumnsWithPosition();
	int getNumPrinted() { return nprinted; }
protected:
	void printColumnsForSrc(DataSource* dataSrc_, int numCols_);
	bool printEntryForColumn(DataSource* dataSrc_, int currCol, int currTotalCol, bool isLast, bool skip_output);
	bool printEntryForColumnToStream(DataSource* dataSrc_, int currCol, int currTotalCol, bool isLast);
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
};

