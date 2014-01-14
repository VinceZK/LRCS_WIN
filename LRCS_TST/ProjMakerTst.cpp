#include "ProjMakerTst.h"
#include "Exception.h"
#include "ProjMaker.h"
#include "ValPos.h"
#include <stdlib.h>

using namespace std;

ProjMakerTst::ProjMakerTst()
{
}


ProjMakerTst::~ProjMakerTst()
{
}

bool ProjMakerTst::run(char* arg1, char* arg2) {
	bool success = true;
	PROJECTION_REC projRec;
	COLUMN_REC colRec;
	vector<COLUMN_REC> columns;
	Projection* proj = NULL;
	char projNameC[MAX_NAME_LENGTH];
	
	try{
		int i = 1;
		string projName;
		ProjMaker* projMaker = new ProjMaker();
		while (i <= 5){
			char* suf = new char[1];
			itoa(i, suf, 10);
			projName = "StringTest0";
			projName += *suf;
			strncpy(projRec.projectionName, projName.c_str(),MAX_NAME_LENGTH);
			strncpy(projRec.primaryColumn, "ID", MAX_NAME_LENGTH);
			strncpy(projRec.belongTable, "$sys$", MAX_NAME_LENGTH);


			columns.clear();
			strncpy(colRec.projectionName, projName.c_str(), MAX_NAME_LENGTH);
			strncpy(colRec.columnName, "ID", MAX_NAME_LENGTH);
			colRec.columnPosition = 1;
			colRec.dataType = ValPos::STRINGTYPE;
			colRec.sorted = true;
			colRec.contiguous = true;
			colRec.compressionType = NO_COMPRESSION;
			colRec.dataLength = 10;
			columns.push_back(colRec);

			strncpy(colRec.projectionName, projName.c_str(), MAX_NAME_LENGTH);
			strncpy(colRec.columnName, "DESC", MAX_NAME_LENGTH);
			colRec.columnPosition = 2;
			colRec.dataType = ValPos::STRINGTYPE;
			colRec.sorted = false;
			colRec.contiguous = false;
			colRec.compressionType = LZO_COMPRESSION;
			colRec.dataLength = 11;
			columns.push_back(colRec);

			proj = new Projection(projRec.projectionName);
			proj->setProjectionRec(&projRec);
			proj->setProjColumns(&columns);
			projMaker->createProjection(proj);
			cout << projName << " created." << endl;
			delete proj;
			i++;
		}
		
		i = 1;
		while (i <= 5){
			char* suf = new char[1];
			itoa(i, suf, 10);
			projName = "StringTest0";
			projName += *suf;
			strncpy(projNameC, projName.c_str(), MAX_NAME_LENGTH);
			Projection* proj = projMaker->getProjection(projNameC);
			if (proj == NULL)return false;
			PROJECTION_REC* projRecPtr = proj->getProjectionRec();
			cout << projRecPtr->projectionName << "|" << projRecPtr->primaryColumn
				<< "|" << projRecPtr->belongTable << "|" << projRecPtr->createTime
				<< "|" << projRecPtr->alterTime << endl;
			vector<COLUMN_REC>* columnsPtr = proj->getProjColumns();
			for (int i = 0; i < columnsPtr->size(); i++)
				cout << columnsPtr->at(i).columnID << "|" << columnsPtr->at(i).projectionName << "|" << columnsPtr->at(i).columnPosition
				<< "|" << columnsPtr->at(i).columnName<< "|" << columnsPtr->at(i).dataType
				<< "|" << columnsPtr->at(i).dataLength << "|" << columnsPtr->at(i).compressionType 
				<< "|" << columnsPtr->at(i).sorted << "|" << columnsPtr->at(i).contiguous 
				<< "|" << columnsPtr->at(i).createTime << endl;
			i++;
		}
		delete projMaker;
	}
	catch (Exception* e){
		cerr << "ProjMakerTst: Error happened" << endl;
		cout << e->msg << endl;
	}
	return success;
}
