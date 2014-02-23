#include "Query1S.h"
#include "ValPos.h"
#include "ProjMaker.h"

Query1S::Query1S() {
}

Query1S::~Query1S() {
}

bool Query1S::run(char* arg1, char* arg2) {
	StopWatch stopWatch;
	Log::writeToLog("Query1S", 10, "Query 1 starting...");
	bool success = true;
	bool createcolumn = false;
	bool forceBuild = false;
	if (*arg1 == '1')
		createcolumn = true;
	else
		createcolumn = false;

	if (*arg2 == '1')
		forceBuild = true;
	else
		forceBuild = false;

	string path = "../data/StringTestData.mid";

	char* cszFile = "StringTest01";
	char* cpProjExt = "proj";

	std::string strProj = cszFile;
	strProj += cpProjExt;

	if (createcolumn){
		Log::writeToLog("Query1S", 10, "Creating Column1...");
		stopWatch.start();

		auto_ptr<ProjMaker> spProjMaker(new ProjMaker());	

		auto_ptr<Projection> spProj(new Projection(strProj.c_str()));

		PROJECTION_REC	projRec;
		vector<COLUMN_REC> columns;
		COLUMN_REC colRecID;
		COLUMN_REC colRecDESC;

		strncpy(projRec.projectionName, strProj.c_str(), MAX_NAME_LENGTH);
		strncpy(projRec.primaryColumn, "ID", MAX_NAME_LENGTH);
		strncpy(projRec.belongTable, "$sys$", MAX_NAME_LENGTH);

		std::string strColName = cszFile;
		strColName += ".ID";
		strncpy(colRecID.projectionName, strProj.c_str(), MAX_NAME_LENGTH);
		strncpy(colRecID.columnName, strColName.c_str(), MAX_NAME_LENGTH);
		colRecID.columnPosition = 1;
		colRecID.dataType = ValPos::STRINGTYPE;
		colRecID.sorted = true;
		colRecID.contiguous = true;
		colRecID.compressionType = NO_COMPRESSION;
		colRecID.dataLength = 10;
		columns.push_back(colRecID);

		SColumnExtracter* ce = new SColumnExtracter(path, colRecID.columnPosition, colRecID.dataLength, forceBuild);
		StringEncoder* encoder = new StringEncoder((Operator*)ce, // data source
			0,              // column index
			NULL,
			colRecID.dataLength,             // string Size
			8 * PAGE_SIZE);   // buffer size in bits
		StringDecoder* decoder = new StringDecoder(colRecID.sorted);         // value sorted
		PagePlacer* pagePlacer = new PagePlacer(encoder,
			decoder,
			2,
			true);
		pagePlacer->placeColumn(strColName, false, colRecID.sorted, 0);


		delete pagePlacer;
		delete ce;
		delete encoder;
		delete decoder;

		Log::writeToLog("Query1S", 10, "Creating Column2...");

		strColName = cszFile;
		strColName += ".DESC";

		strncpy(colRecDESC.projectionName, strProj.c_str(), MAX_NAME_LENGTH);
		strncpy(colRecDESC.columnName, strColName.c_str(), MAX_NAME_LENGTH);
		colRecDESC.columnPosition = 2;
		colRecDESC.dataType = ValPos::STRINGTYPE;
		colRecDESC.sorted = false;
		colRecDESC.contiguous = false;
		colRecDESC.compressionType = NO_COMPRESSION;
		colRecDESC.dataLength = 11;
		columns.push_back(colRecDESC);

		//string path=RLE_EXTRACT_LOAD_AND_ACCESS_DATA_PATH;
		ce = new SColumnExtracter(path, colRecDESC.columnPosition, colRecDESC.dataLength, forceBuild);
		encoder = new StringEncoder((Operator*)ce, // data source
			0,              // column index
			NULL,
			colRecDESC.dataLength,             // string Size
			8 * PAGE_SIZE);   // buffer size in bits
		decoder = new StringDecoder(colRecDESC.sorted);         // value sorted
		pagePlacer = new PagePlacer(encoder,
			decoder,
			2,
			true);
		pagePlacer->placeColumn(strColName, false, colRecDESC.sorted, 0);
		delete pagePlacer;
		delete ce;
		delete encoder;
		delete decoder;
		stopWatch.stop();
		//system("move StringTest01* " RUNTIME_DATA);

		spProj->setProjectionRec(&projRec);
		spProj->setProjColumns(&columns);
		spProjMaker->createProjection(spProj.get());
	}

	Log::writeToLog("Query1S", 10, "Opening Column1...");
	stopWatch.start();

	auto_ptr<ProjMaker> spProjMaker(new ProjMaker());

	char szProj[MAX_NAME_LENGTH];
	
	strncpy(szProj, strProj.c_str(), MAX_NAME_LENGTH);
	Projection* proj = spProjMaker->getProjection(szProj);


	PROJECTION_REC* projRecPtr = proj->getProjectionRec();
	vector<COLUMN_REC>* columnsPtr = proj->getProjColumns();

	//delete spProjMaker;
	
	ROSAM* am1 = new ROSAM((*columnsPtr)[0].columnName, 2, sizeof(int), (*columnsPtr)[0].dataLength, ValPos::INTTYPE, (*columnsPtr)[0].dataType);
	Log::writeToLog("Query1S", 10, "Opening Column2...");
	ROSAM* am2 = new ROSAM((*columnsPtr)[1].columnName, 2, sizeof(int), (*columnsPtr)[1].dataLength, ValPos::INTTYPE, (*columnsPtr)[1].dataType);
	Predicate* pred1 = new Predicate(Predicate::OP_GREATER_THAN_OR_EQUAL);
	Predicate* pred2 = new Predicate(Predicate::OP_LESS_THAN);
	StringDataSource* ds1 = new StringDataSource(am1, (*columnsPtr)[0].sorted, true);
	StringDataSource* ds2 = new StringDataSource(am2, (*columnsPtr)[1].sorted, true);

	ds1->setPredicate(pred1);
	ds2->setPredicate(pred2);

	ValPos* rhs1 = new StringValPos(10);
	char* RHSVal1 = "abcde99990";
	rhs1->set((byte*)RHSVal1);
	ds1->changeRHSBinding(rhs1);

	ValPos* rhs2 = new StringValPos(11);
	char* RHSVal2 = "Cstore00049";
	rhs2->set((byte*)RHSVal2);
	ds2->changeRHSBinding(rhs2);
	//Count* agg = new Count((Operator*) ds1, 0, (Operator*) ds1, 0);


	//ds1->printColumn();
	//ds2->printColumn();

	DataSource* srcs[2] = { ds1, ds2 };
	int numCols[2] = { 1, 1 };

	BlockPrinter* bPrint = new BlockPrinter(srcs, numCols, 2, NULL);
	bPrint->printColumns();
	cout << "Query 1 S took: " << stopWatch.stop() << " ms" << endl;
	delete ds1;
	delete am1;
	delete rhs1;
	delete pred1;
	delete ds2;
	delete am2;
	delete rhs2;
	delete pred2;
	delete bPrint;
	return success;
}