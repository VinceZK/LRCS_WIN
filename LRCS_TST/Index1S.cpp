#include "Index1S.h"
#include "StopWatch.h"
#include "log.h"
#include "BDBEnv.h"


/*Test incremental indexing based on middle size data*/
Index1S::Index1S()
{
}


Index1S::~Index1S()
{
}

bool Index1S::run(char* arg1, char* arg2) {
	StopWatch stopWatch;
	Log::writeToLog("Index1S", 10, "Index 1 starting...");
	bool success = true;
	bool forceBuild = false;
	if (*arg1 == '1')
		forceBuild = true;
	else
		forceBuild = false;
	string path1 = "../data/StringTestData.01.tiny";
	string path2 = "../data/StringTestData.02.tiny";

	stopWatch.start();
	try{
		//index(path1, forceBuild);
		index(path2, forceBuild);
	}
	catch (Exception* e){
		cout << e->msg << endl;
		success = false;
	}
	stopWatch.stop();
	return success;
}

void Index1S::index(string path, bool forceBuild){
	Log::writeToLog("Index1S", 10, "Creating Column...");
	//string dbName = strcat(RUNTIME_DATA, "StringTest01.ID");
	char* dbFullName = new char[1024];
	string dbName = "StringTest01.ID";
	strcpy(dbFullName, (RUNTIME_DATA + dbName).c_str());
	ce = new SColumnExtracter(path, 1, 10, forceBuild);
	encoder = new StringEncoder((Operator*)ce, // data source
								0,    // column index
								NULL,
								10,    // string Size
								8 * PAGE_SIZE);   // buffer size in bits
	decoder = new StringDecoder(true);         // value sorted
	pagePlacer = new PagePlacer(encoder,
								decoder,
								2,
								false);
	pagePlacer->placeColumn(dbName, false, true,0);
	delete pagePlacer;
	delete ce;
	delete encoder;
	delete decoder;

	dbName = "StringTest01.DESC";
	strcpy(dbFullName, (RUNTIME_DATA + dbName).c_str());
	ce = new SColumnExtracter(path, 2, 11, forceBuild);
	encoder = new StringEncoder((Operator*)ce, // data source
								0,              // column index
								NULL,
								11,             // string Size
								8 * PAGE_SIZE);   // buffer size in bits
	decoder = new StringDecoder(false);         // value sorted
	pagePlacer = new PagePlacer(encoder,
								decoder,
								2,
								false);
	pagePlacer->placeColumn(dbName, false, false,0);
	delete pagePlacer;
	delete ce;
	delete encoder;
	delete decoder;
}