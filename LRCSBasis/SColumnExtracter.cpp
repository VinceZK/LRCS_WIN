#include "SColumnExtracter.h"
#include "StringUtil.h"
//#include <fstream>
using namespace std;

SColumnExtracter::SColumnExtracter(string fileName, int c_index, int stringSize_, bool FORCE_REBUILD)
{
	svp = new StringValPos((unsigned short)stringSize_);
	bb = new BasicBlock(true, true, true, svp);
	curPos = 1;
	stringSize = stringSize_;
	curVal = new char[stringSize];

	string splitedColumn = extractColumn(fileName, c_index, FORCE_REBUILD);
	infile = new ifstream(splitedColumn.c_str());


}

string SColumnExtracter::extractColumn(string fileName, int index, bool FORCE_REBUILD)
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
			//extractColumnFromFile(fileName, index, s);
			throw new UnexpectedException("The source file can not be open!");
		else
			delete fin;
	}
	return s;
}


SColumnExtracter::~SColumnExtracter()
{
	delete infile;
	delete[] curVal;
	delete svp;
	delete bb;
}

Block* SColumnExtracter::getNextValBlock(int colIndex_) {
	if (colIndex_ != 0) return NULL;
	string line;
	int size;
	if (!(getline(*infile, line)))
		return NULL;
	size = line.size();
	if (stringSize > size){//set the right part ZERO
		memset(svp->value + size, 0, stringSize - size);
		memcpy(svp->value, line.c_str(), size);
	}
	else{
		memcpy(svp->value, line.c_str(), stringSize);
	}
	svp->position = curPos;
	curPos++;
	bb->setValue(svp);
	return bb;

}


PosBlock* SColumnExtracter::getNextPosBlock(int colIndex_) {
	if (colIndex_ != 0) return NULL;
	return NULL;
}

void SColumnExtracter::extractColumnFromFile(string fileNameIn, int index, string fileNameOut){
	string line;
	char* sep = " "; //SPACE
	ifstream* fin = new ifstream(fileNameIn.c_str());

	if (!fin->is_open())
		throw UnexpectedException("Error opening file!");

	ofstream fout(fileNameOut);
	if (!fout)
		throw UnexpectedException("Error saving file!");

	while (!fin->eof()){		
		if(!(getline(*fin, line)))break;
		//if (line.length() == 0)exit;
		char* lineChar = const_cast<char*>(line.c_str());
		fout << StringUtil::extractDelimitedSubstring(sep, lineChar, index) << endl;
	}

	//fout << ColumnOut;
	fin->close();
	fout.close();
}
