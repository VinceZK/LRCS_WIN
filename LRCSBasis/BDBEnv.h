#pragma once
#include <db_cxx.h>
using namespace std;
static DbEnv* dbenv;
class __declspec(dllexport) BDBEnv
{
public:
	BDBEnv();
	~BDBEnv();
	static void initDbEnv(const char* home);
	static DbEnv* getDbEnv();
	static void closeDbEnv();
	static void teardown(const char* home);
private:
	static char* home;
};
