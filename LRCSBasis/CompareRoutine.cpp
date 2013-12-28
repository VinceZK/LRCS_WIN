#include "CompareRoutine.h"
#include <assert.h>
using namespace std;

CompareRoutine::CompareRoutine()
{
}


CompareRoutine::~CompareRoutine()
{
}

int CompareRoutine::compare_int(Db *dbp, const Dbt *a, const Dbt *b)
{
	int i = *((int *)a->get_data());
	int j = *((int *)b->get_data());

	if (i > j)
		return (1);
	if (i < j)
		return (-1);
	return (0);
}
int CompareRoutine::compare_double(Db *dbp, const Dbt *a, const Dbt *b)
{
	double i = *((double *)a->get_data());
	double j = *((double *)b->get_data());

	if (i > j)
		return (1);
	if (i < j)
		return (-1);
	return (0);
}

int CompareRoutine::compare_key(Db *dbp, const Dbt *a, const Dbt *b)
{
	char* i = ((char *)a->get_data());
	char* j = ((char *)b->get_data());
	int size1 = a->get_size();
	int size2 = b->get_size();
	/*if (size1 != size2){
		cout << size1 << endl;
		cout << size2 << endl;
	}*/
	//assert(size1 == size2);
	return memcmp(i, j, size1);
}