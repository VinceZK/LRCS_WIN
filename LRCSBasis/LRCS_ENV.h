#pragma once
#include <map>
using namespace std;

class __declspec(dllexport) LRCS_ENV
{
public:
	LRCS_ENV();
	~LRCS_ENV();
	static bool initLRCS();
	static void closeLRCS();
	static string getGlobalConf(string name_);
	static void setGlobalConf(string name_, string value_);
private:
	static void initConfigurations();
	static map<string, string>*  LRCS_CONF;
};

