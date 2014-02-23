#include "stdafx.h"
#include "LRCS_ENV.h"
#include "Constants.h"
#include "Exception.h"
#include "SysDbEnv.h"
#include "BDBEnv.h"
#include <fstream>
#include <sstream>
#include "StringUtil.h"
#include "Log.h"

#define DEFAULT_GLOB_CONF "../SystemDB/LRCS_CONFIG"
#define SEPARATOR '\t'

map<string, string>*  LRCS_ENV::LRCS_CONF;

LRCS_ENV::LRCS_ENV()
{
}


LRCS_ENV::~LRCS_ENV()
{
}

bool LRCS_ENV::initLRCS(){
	try{
		initConfigurations();
		cout << "LRCS global configurations are loaded!" << endl;
		Log::logInit();
		cout << "LRCS log are initialized!" << endl;
		SysDbEnv::initDbEnv();
		cout << "System DB enviroment is initialized!" << endl;
		BDBEnv::initDbEnv(RUNTIME_DATA);
		cout << "Index DB enviroment is initialized!" << endl;
	}
	catch (Exception& e){
		cerr << e.msg << endl;
		return false;
	}
	return true;
}

void LRCS_ENV::closeLRCS(){
	try{
		BDBEnv::closeDbEnv();
		SysDbEnv::closeDbEnv();
		Log::logDestroy();
		delete LRCS_CONF;
	}
	catch (Exception& e){
		cerr << e.msg << endl;
	}
}

void LRCS_ENV::initConfigurations(){
	string LRCSConfig = DEFAULT_GLOB_CONF;
	ifstream* infile = new ifstream(LRCSConfig.c_str());
	LRCS_CONF = new map<string, string>;
	string line;
	string whitespace = "\t ";
	while (getline((*infile), line)){
		//use # indicate comment for test config File
		line = StringUtil::ltrim(line, whitespace);
		if (line[0] != '#' && line.size() > 1){
			//according to global config file
			//First token is the variable name
			//second token is the value
			string varName = StringUtil::splitDelimitedSubstring(SEPARATOR, line, 1);
			string varValue = StringUtil::splitDelimitedSubstring(SEPARATOR, line, 2);
			//cout << varName << " = " << varValue << endl;
			//LRCS_CONF[varName] = varValue;
			setGlobalConf(varName, varValue);
		}
	}
	delete infile;
}

string LRCS_ENV::getGlobalConf(string name_){
	map<string, string>::iterator m_it;
	m_it = LRCS_CONF->find(name_);
	if (m_it == LRCS_CONF->end())return " ";
	else return m_it->second;

}

void LRCS_ENV::setGlobalConf(string name_, string value_){
	map<string, string>::iterator m_it;
	m_it = LRCS_CONF->find(name_);
	if (m_it == LRCS_CONF->end()){
		LRCS_CONF->insert(map<string, string>::value_type(name_, value_));
	}
	else{
		m_it->second = value_;
	}
}