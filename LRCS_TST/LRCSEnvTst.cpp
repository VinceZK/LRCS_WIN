#include "LRCSEnvTst.h"
#include "LRCS_ENV.h"

LRCSEnvTst::LRCSEnvTst()
{
}


LRCSEnvTst::~LRCSEnvTst()
{
}

bool LRCSEnvTst::run(char* arg1, char* arg2){
	//LRCS_ENV::initLRCS();
	LRCS_ENV::setGlobalConf("FORCE_BUILD", "0");
	LRCS_ENV::setGlobalConf("LOG_PRIORITY_THRESHOLD", "1");
	cout << "FORCE_BUILD = " << LRCS_ENV::getGlobalConf("FORCE_BUILD") << endl;
	cout << "LOG_PRIORITY_THRESHOLD = " << LRCS_ENV::getGlobalConf("LOG_PRIORITY_THRESHOLD") << endl;
	cout << "NO_LOG = " << LRCS_ENV::getGlobalConf("NO_LOG") << endl;
	cout << "SPLIT = " << LRCS_ENV::getGlobalConf("SPLIT") << endl;
	return true;
}
