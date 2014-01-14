#include "LogTst.h"
#include "LRCS_ENV.h"
#include "Log.h"
/*
*Test Log utilization.
*The log util can be configured in LRCS configuration file: ../SystemDB/LRCS_CONFIG
*LOG_PRIORITY_THRESHOLD	10 :Only the priority >= the threshold set here should the log work
*LOG_TO_FILE	TRUE :Whether the log is save in a file or show on console
*LOG_FILENAME	LRCSRuntime.log :log file name
*LOG_SIZE	10  :log file size (MB). After the log file reach the size specified here, it will move to the next slot.                    
*NO_LOG	FALSE   :Globly deactive the log utilization.
*/
LogTst::LogTst()
{
}


LogTst::~LogTst()
{
}

bool LogTst::run(char* arg1, char* arg2){
	//LRCS_ENV::initLRCS();
	//Log::logInit();
	int i = 1;
	char* msg = new char[30];
	Log::writeToLog("PageWriter.cpp", 10, "Column %s is initialized, transaction enable is %s",
		"StringTest01.DESC", true ? "true" : "false");
	while (i <= 20){
		//Test int
		Log::writeToLog("LogTst.cpp", 10, "Log Test in LRCS ", i);
		Log::writeToLog("LogTst.cpp", 10, "Log Test in LRCS %d, %d", i, 10*i);
	
		//Test boolean
		bool testb = true;
		Log::writeToLog("LogTst.cpp", 10, "Log Test in LRCS %s", testb ? "true" : "false");

		//Test Char*
		char* testc = "Test char";
		Log::writeToLog("LogTst.cpp", 10, "Log Test in LRCS %s", testc);

		//Test string
		string tests = "Test string";
		Log::writeToLog("LogTst.cpp", 10, "Log Test in LRCS %s %s", tests.c_str(), testc);

		i++;
	}
	delete[] msg;
	//Log::logDestroy();
	return true;
}

