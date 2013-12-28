#pragma once
#include "Projection.h"
#include <ostream>
#include <db_cxx.h>
#define SYS_PROJECTION "$SYS$PROJECTION"
#define SYS_COLUMN "$SYS$COLUMN"
#define SYS_COLUMN_SECONDARY "$SYS$COLUMN~01"/*secondary index of column*/

/*Factory Class to generate/get projections*/
class __declspec(dllexport) ProjMaker
{
public:
	ProjMaker();
	~ProjMaker();
	void createProjection(PROJECTION_REC*  projection_, vector<COLUMN_REC>* columns_);
	void createProjection(Projection* proj_);
	Projection* getProjection(char* projName_);	
private:
	static int getProjName(Db* pDb_, const Dbt* pkey_, const Dbt* data_, Dbt* skey_);
	void initDbStruc(DbTxn* tid);
	void closeDbStruc();

	bool initialized;
	DbEnv* sysdbenv;
	DbTxn* tid;
	Db* sysProjection;      /*$SYS$PROJECTION*/
	Db* sysColumn;          /*$SYS$COLUMN*/
	Db* sysColumnSecondary; /*secondary index of DB: column*/
};

