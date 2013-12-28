#pragma once
#include "Constants.h"
#include "Operator.h"
#include "BasicBlock.h"
#include "StringValPos.h"
#include <string>
#include <fstream>
#include <sstream>
#include <map>
using namespace std;

class __declspec(dllexport) SColumnExtracter : public Operator
{
public:
	SColumnExtracter(string fileName, int c_index, int stringSize_, bool FORCE_REBUILD);
	// destructor
	~SColumnExtracter();

	//takes a file and extracts just one column from it
	string extractColumn(string fileName, int index, bool FORCE_REBUILD);

	// Gets the next value block from the operator 
	Block* getNextValBlock(int colIndex_);

	// Gets the next position block (bitstring of positions) from the operator
	PosBlock* getNextPosBlock(int colIndex_);

	inline virtual int getLastPosition() { throw UnimplementedException("Not applicable for SColumnExtracter"); };

private:
	int curPos;
	int stringSize;
	char* curVal;
	BasicBlock* bb;
	StringValPos* svp;
	ifstream* infile;
	void extractColumnFromFile(string fileNameIn, int index, string fileNameOut);
	//char* extractDelimitedSubstring(char* d, char* strLine, int index);
};

