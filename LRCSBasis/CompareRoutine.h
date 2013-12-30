#pragma once

#ifdef LRCSBASIS_EXPORTS
#define LRCSBasis_API __declspec(dllexport)
#else
#define LRCSBasis_API __declspec(dllimport)
#endif

#include <db_cxx.h>

class LRCSBasis_API CompareRoutine
{
public:
	CompareRoutine();
	~CompareRoutine();
	static  int  compare_int(Db *dbp, const Dbt *a, const Dbt *b);
	static  int  compare_key(Db *dbp, const Dbt *a, const Dbt *b);
	static  int  compare_double(Db *dbp, const Dbt *a, const Dbt *b);
};

