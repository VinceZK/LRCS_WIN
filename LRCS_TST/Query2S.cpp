#include "Query2S.h"
#include "ValPos.h"
#include "LZEncoder.h"
#include "LZDecoder.h"
#include "LZDataSource.h"
/**
* Query2S:
* SELECT *
* FROM StringTest03
*
* Projections: (StringTest03.ID| StringTest03.DESC)
*
* Purpose:
* Test string data selection with LZ compress
*/
using namespace std;

Query2S::Query2S()
{
}


Query2S::~Query2S()
{
}

bool Query2S::run(char* arg1, char* arg2) {
	StopWatch stopWatch;
	Log::writeToLog("Query2S", 10, "Query 1 starting...");
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

	string path = "../data/StringTestData.large";

	if (createcolumn){
		Log::writeToLog("Query2S", 10, "Creating Column1...");
		stopWatch.start();
		SColumnExtracter* ce = new SColumnExtracter(path, 1, 10, forceBuild);
		StringEncoder* encoder = new StringEncoder((Operator*)ce, // data source
			0,              // column index
			NULL,
			10,             // string Size
			20 * PAGE_SIZE);   // buffer size in bits
		StringDecoder* decoder = new StringDecoder(true);         // value sorted
		LZEncoder* lzEncoder = new LZEncoder(encoder); //Compress the data
		LZDecoder* lzDecoder = new LZDecoder(decoder); //Decompress the data
		PagePlacer* pagePlacer = new PagePlacer(lzEncoder,
			lzDecoder,
			2,
			true);
		pagePlacer->placeColumn("StringTest02.ID", false, true,0);
		delete pagePlacer;
		delete ce;
		delete encoder;
		delete decoder;
		delete lzEncoder;
		delete lzDecoder;

		Log::writeToLog("Query2S", 10, "Creating Column2...");
		//string path=RLE_EXTRACT_LOAD_AND_ACCESS_DATA_PATH;
		ce = new SColumnExtracter(path, 2, 11, forceBuild);
		encoder = new StringEncoder((Operator*)ce, // data source
			0,              // column index
			NULL,           // position encoder
			11,             // string Size
			20 * PAGE_SIZE);   // buffer size in bits
		decoder = new StringDecoder(true);         // value sorted
		lzEncoder = new LZEncoder(encoder); //Compress the data
		lzDecoder = new LZDecoder(decoder); //Decompress the data
		pagePlacer = new PagePlacer(lzEncoder,
			lzDecoder,
			2,
			true);
		pagePlacer->placeColumn("StringTest02.DESC", false, true,0);
		delete pagePlacer;
		delete ce;
		delete encoder;
		delete decoder;
		delete lzEncoder;
		delete lzDecoder;

		stopWatch.stop();
		//system("move StringTest02* " RUNTIME_DATA);	
	}

	Log::writeToLog("Query2S", 10, "Opening Column1...");
	
	stopWatch.start();

	ROSAM* am1 = new ROSAM("StringTest02.ID", 2, sizeof(int), 10, ValPos::INTTYPE, ValPos::STRINGTYPE);
	ROSAM* am2 = new ROSAM("StringTest02.DESC", 2, sizeof(int), 11, ValPos::INTTYPE, ValPos::STRINGTYPE);
	Predicate* pred1 = new Predicate(Predicate::OP_LESS_THAN);
	Predicate* pred2 = new Predicate(Predicate::OP_LESS_THAN);
	LZDataSource* ds1 = new LZDataSource(am1, true, true, (new StringDecoder(true)));
	LZDataSource* ds2 = new LZDataSource(am2, true, true, (new StringDecoder(true)));

	ds1->setPredicate(pred1);
	ds2->setPredicate(pred2);

	ValPos* rhs1 = new StringValPos(10);
	char* RHSVal1 = "abc0010001";
	rhs1->set((byte*)RHSVal1);
	ds1->changeRHSBinding(rhs1);

	ValPos* rhs2 = new StringValPos(11);
	char* RHSVal2 = "LRCS0000051";
	rhs2->set((byte*)RHSVal2);
	ds2->changeRHSBinding(rhs2);

	DataSource* srcs[2] = { ds1, ds2 };
	int numCols[2] = { 1, 1 };


	BlockPrinter* bPrint = new BlockPrinter(srcs, numCols, 2, "Query2S.out");
	bPrint->printColumns();

	cout << "Query 1 S took: " << stopWatch.stop() << " ms" << endl;

	delete ds1;
	delete am1;
	delete rhs1;
	delete ds2;
	delete am2;
	delete rhs2;
	delete bPrint;
	return success;
}

