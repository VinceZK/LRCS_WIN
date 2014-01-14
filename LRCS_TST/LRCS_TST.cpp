// LRCS_TST.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "LRCS_ENV.h"
#include <assert.h>
#include <string>
#include <map>
#include "UnitTest.h"
#include "Query1S.h"
#include "Query2S.h"
#include "Query3S.h"
#include "Query4S.h"
#include "ExceptionTst.h"
#include "ProjMakerTst.h"
#include "LRCSEnvTst.h"
#include "LogTst.h"

using namespace std;

void runTest(string, char*, char*);
void initAllTestSuites(map<string, UnitTest*> &testSuites);

int main(int argc, CHAR* argv[])
{
	using namespace std;
	if (!LRCS_ENV::initLRCS()){
		cerr << "LRCS enviroment initialization failed!" << endl;
		return 0;
	}
		
	string testName;
	bool useOwn = true;
	char* arg1 = NULL;
	char* arg2 = NULL;

	/*if (argc >= 2){
		useOwn = (!(argv[1][0] == '0'));
		if (useOwn) {
			if (argc >= 3) {
				arg1 = argv[2];
				if (argc >= 4)
					arg2 = argv[3];
			}
		}
		else {
			//assert(argc >= 4);
			testName = argv[3];
			if (argc >= 5) {
				arg1 = argv[4];
				if (argc >= 6)
					arg2 = argv[5];
			}
		}
	}*/
	if (argc == 2){
		testName = argv[1];
		arg1 = "0";
		arg2 = "0";
	}
	else if (argc == 3){
		arg1 = argv[1];
		arg2 = "0";
		testName = argv[2];
	}
	else if (argc >= 4){
		arg1 = argv[1];
		arg2 = argv[2];
		testName = argv[3];
	}
	else{
		testName = "Query1S";
		arg1 = "0";
		arg2 = "0";
	}
	//cout << "args: " << arg1 << " " <<  arg2 << endl;

	cout << "------------------------Begin Test-----------------------------" << endl;
	runTest(testName, arg1, arg2);
	cout << "------------------End Test & Recall Resource-------------------" << endl;
	LRCS_ENV::closeLRCS();
	cout << "----------------------Resource Recalled------------------------" << endl;
	return 0;
}

void runTest(string testName, char* arg1, char* arg2){
	bool success = true;
	map<string, UnitTest*> testSuites;
	initAllTestSuites(testSuites);

	cout << "TEST SUIT: " << testName << " Loaded!" << endl;
	if (testSuites[testName] != NULL)
		success &= testSuites[testName]->run(arg1, arg2);

	if (success)
		cout << endl << "SUCCESSFULL ON ALL SUITES" << endl;
	else
		cout << endl << "FAILED SOME SUITES" << endl;

	for (map<string, UnitTest*>::iterator iterPos = testSuites.begin(); iterPos != testSuites.end(); ++iterPos)
		delete iterPos->second;
}

void initAllTestSuites(map<string, UnitTest*> &testSuites){
	testSuites["Query1S"] = new Query1S();
	testSuites["Query2S"] = new Query2S();
	testSuites["Query3S"] = new Query3S();
	testSuites["Query4S"] = new Query4S();
	testSuites["ExceptionTst"] = new ExceptionTst();
	testSuites["ProjMakerTst"] = new ProjMakerTst();	
	testSuites["LRCSEnvTst"] = new LRCSEnvTst();
	testSuites["LogTst"] = new LogTst();	
}