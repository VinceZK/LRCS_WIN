
#include "Query3S.h"
#include "ValPos.h"
#include "LZEncoder.h"
#include "LZDecoder.h"
#include "LZDataSource.h"
#include "BitmapEncoder.h"
#include "BitmapDecoder.h"
#include "BitmapDataSource.h"
/**
* Query3S:
* SELECT *
* FROM StringTest03
*
* Projections: (StringTest03.ID| StringTest03.DESC)
*
* Purpose:
* Test bitmap encoding
*/
Query3S::Query3S() {
}

Query3S::~Query3S() {
}

bool Query3S::run(char* arg1, char* arg2) {
	StopWatch stopWatch;
	Log::writeToLog("Query3S", 10, "Query 1 starting...");
	bool success = true;
	bool createcolumn;
	bool forceBuild;

	if (*arg1 == '1')
		createcolumn = true;
	else
		createcolumn = false;

	if (*arg2 == '1')
		forceBuild = true;
	else
		forceBuild = false;

	string path = "../data/BitMapTestData.large";

	if (createcolumn){
		Log::writeToLog("Query3S", 10, "Creating Column1...");
		stopWatch.start();
		SColumnExtracter* ce = new SColumnExtracter(path, 1, 10, forceBuild);
		//BitmapEncoder* posEncoder = new BitmapEncoder(10, 20*PAGE_SIZE);
		StringEncoder* encoder = new StringEncoder((Operator*)ce, // data source
			0,              // column index
			NULL,     //position encoder
			10,             // string Size
			20 * PAGE_SIZE);  // buffer size in bits 2.5
		StringDecoder* decoder = new StringDecoder(true);          // value sorted
		LZEncoder* lzEncoder = new LZEncoder(encoder); //Compress the data
		LZDecoder* lzDecoder = new LZDecoder(decoder); //Decompress the data
		PagePlacer* pagePlacer = new PagePlacer(lzEncoder,
			lzDecoder,
			2,
			true);
		pagePlacer->placeColumn("StringTest03.ID", false, true,0);
		delete pagePlacer;
		delete ce;
		delete encoder;
		delete decoder;
		delete lzEncoder;
		delete lzDecoder;

		Log::writeToLog("Query3S", 10, "Creating Column2...");
		ce = new SColumnExtracter(path, 2, 4, forceBuild);
		BitmapEncoder* posEncoder = new BitmapEncoder(4, 8 * PAGE_SIZE);
		encoder = new StringEncoder((Operator*)ce, // data source
			0,              // column index
			posEncoder,     // position encoder
			4,             // string Size
			16 * PAGE_SIZE);   // buffer size in bits
		decoder = new StringDecoder(true);         // value sorted
		lzEncoder = new LZEncoder(encoder); //Compress the data
		lzDecoder = new LZDecoder(decoder); //Decompress the data
		pagePlacer = new PagePlacer(lzEncoder,
			lzDecoder, 
			2,
			true);
		pagePlacer->placeColumn("StringTest03.CODE", false, false,0);
		delete pagePlacer;
		delete ce;
		delete posEncoder;
		delete encoder;
		delete decoder;
		delete lzEncoder;
		delete lzDecoder;
		stopWatch.stop();
		//system("move StringTest03* " RUNTIME_DATA);
	}

	Log::writeToLog("Query3S", 10, "Opening Column1...");
	
	stopWatch.start();

	ROSAM* am1 = new ROSAM("StringTest03.ID", 2, sizeof(int), 10, ValPos::INTTYPE, ValPos::STRINGTYPE);
	ROSAM* am2 = new ROSAM("StringTest03.CODE", 2, sizeof(int), 4, ValPos::INTTYPE, ValPos::STRINGTYPE);
	Predicate* pred1 = new Predicate(Predicate::OP_LESS_THAN);
	Predicate* pred2 = new Predicate(Predicate::OP_LESS_THAN_OR_EQUAL);
	LZDataSource* ds1 = new LZDataSource(am1, true, true, (new StringDecoder(true)));
	BitmapDataSource* ds2 = new BitmapDataSource(am2, false, true, (new StringDecoder(true)));

	ValPos* rhs1 = new StringValPos(10);
	char* RHSVal1 = "abc0000040";
	rhs1->set((byte*)RHSVal1);
	ValPos* rhs2 = new StringValPos(4);
	char* RHSVal2 = "2000";
	rhs2->set((byte*)RHSVal2);

	//ds1->setPredicate(pred1);
	//ds1->changeRHSBinding(rhs1);
	ds2->setPredicate(pred2);
	ds2->changeRHSBinding(rhs2);

	DataSource* srcs[2] = { ds1, ds2 };
	int numCols[2] = { 1, 1 };


	BlockPrinter* bPrint = new BlockPrinter(srcs, numCols, 2, "Query3S.out");
	cout << "Query 3 S took: " << stopWatch.stop() << " ms" << endl;

	bPrint->printColumns();
	delete ds1;
	delete am1;
	delete ds2;
	delete am2;
	return success;
}
