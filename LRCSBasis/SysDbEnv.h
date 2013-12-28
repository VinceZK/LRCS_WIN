#pragma once
#include <db_cxx.h>
using namespace std;

namespace NS_SYSDBENV{
	static DbEnv* sysdbenv = NULL;
}

class __declspec(dllexport) SysDbEnv
{
public:
	SysDbEnv();
	~SysDbEnv();
	static void initDbEnv();
	static DbEnv* getDbEnv();
	static void closeDbEnv();
	static void teardown(const char* home);
private:
	//static char* home;
};

