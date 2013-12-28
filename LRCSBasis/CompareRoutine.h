#pragma once
#include <db_cxx.h>

class  CompareRoutine
{
public:
	CompareRoutine();
	~CompareRoutine();
	static  int  compare_int(Db *dbp, const Dbt *a, const Dbt *b);
	static  int  compare_key(Db *dbp, const Dbt *a, const Dbt *b);
	static  int  compare_double(Db *dbp, const Dbt *a, const Dbt *b);
};

