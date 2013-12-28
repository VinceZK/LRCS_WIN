#include "ColumnExtracter.h"
#include "StringUtil.h"
using namespace std;

ColumnExtracter::ColumnExtracter(string fileName, int c_index, bool FORCE_REBUILD)
{
	bb = new BasicBlock(true, true, true);
	//assume int type for now ... fix later
	p = new IntValPos();
	curPos = 1;


	string sortedColumn = extractColumn(fileName, c_index, FORCE_REBUILD);
	infile = new ifstream(sortedColumn.c_str());


}

string ColumnExtracter::extractColumn(string fileName, int index, bool FORCE_REBUILD)
{
	char* buff = new char[256];
	sprintf(buff, "%s.%d", fileName.c_str(), index);
	string s = buff;
	delete[] buff;

	/*if (FORCE_REBUILD)
	sprintf(buff, "cat %s | awk '{print $%d}' > %s.%d", fileName.c_str(), index, fileName.c_str(), index);
	else
	sprintf(buff, "./filechecker.sh %s %d %s.%d", fileName.c_str(), index, fileName.c_str(), index);
	//if test ! -f $3; then echo "{print \$$2}" > dna.awk; cat $1 | awk -f dna.awk > $3; rm dna.awk; fi;*/
	if (FORCE_REBUILD){
		extractColumnFromFile(fileName, index, s);
	}
	else{
		ifstream* fin = new ifstream(s.c_str()); //Check if column file exists
		if (!fin->is_open())
			extractColumnFromFile(fileName, index, s);
	}
	return s;
}


ColumnExtracter::~ColumnExtracter()
{
	delete bb;
	delete infile;
	delete p;
	//delete bb;
	//delete infile;
}

Block* ColumnExtracter::getNextValBlock(int colIndex_) {
	if (colIndex_ != 0) return NULL;
	string line;
	int curVal;
	if (!(getline(*infile, line)))
		return NULL;
	curVal = atoi(line.c_str());
	p->set(curPos, (byte*)&curVal);
	//p->value = curVal;
	//p->position = curPos;
	curPos++;
	bb->setValue(p);
	return bb;
}

PosBlock* ColumnExtracter::getNextPosBlock(int colIndex_) {
	if (colIndex_ != 0) return NULL;
	return NULL;
}

void ColumnExtracter::extractColumnFromFile(string fileNameIn, int index, string fileNameOut){
	string line;
	char* sep = " "; //SPACE
	ifstream* fin = new ifstream(fileNameIn.c_str());

	if (!fin->is_open())
		throw UnexpectedException("Error opening file!");

	ofstream fout(fileNameOut);
	if (!fout)
		throw UnexpectedException("Error saving file!");

	while (!fin->eof()){
		if (!(getline(*fin, line)))break;
		//if (line.length() == 0)exit;
		char* lineChar = const_cast<char*>(line.c_str());
		fout << StringUtil::extractDelimitedSubstring(sep, lineChar, index) << endl;
	}

	//fout << ColumnOut;
	fin->close();
	fout.close();
}