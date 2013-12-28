#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include "Constants.h"
#include "Operator.h"
#include "ValPos.h"
#include "BasicBlock.h"
using namespace std;

class Block;

class __declspec(dllexport) ColumnExtracter : public Operator
{
public:
	ColumnExtracter(string fileName, int c_index, bool FORCE_REBUILD);
	// destructor
	~ColumnExtracter();

	//takes a file and extracts just one column from it
	string extractColumn(string fileName, int index, bool FORCE_REBUILD);

	// Gets the next value block from the operator 
	Block* getNextValBlock(int colIndex_);

	// Gets the next position block (bitstring of positions) from the operator
	PosBlock* getNextPosBlock(int colIndex_);

	inline virtual int getLastPosition() { throw UnimplementedException("Not applicable for ColumnExtracter"); };

private:
	int curPos;
	int curVal;
	BasicBlock* bb;
	ValPos* p;
	ifstream* infile;
	void extractColumnFromFile(string fileNameIn, int index, string fileNameOut);
};

