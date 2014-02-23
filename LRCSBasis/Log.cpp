#include "Log.h"
#include "TypeConvert.h"
#include "LRCS_ENV.h"
#include "SysTime.h"
#include "StringUtil.h"
#include "UnexpectedException.h"



using namespace std;

int Log::threshold;
int Log::logSize;
ostream* Log::logStream;
string Log::logFileName;
bool Log::log_2_file;
bool Log::no_log = true;
map<int, string>* Log::logSlot;
CRITICAL_SECTION Log::csLock;
char* Log::logstr;

Log::Log()
{}
Log::~Log()
{}

void Log::logInit() {
	log_2_file = TypeConvert::StringToBoolean(LRCS_ENV::getGlobalConf("LOG_TO_FILE"));
	logFileName = LRCS_ENV::getGlobalConf("LOG_FILENAME");
	if (logFileName.empty())logFileName = "LRCSRuntime.log";
	threshold = TypeConvert::StringToInt(LRCS_ENV::getGlobalConf("LOG_PRIORITY_THRESHOLD"));
	logSize = TypeConvert::StringToInt(LRCS_ENV::getGlobalConf("LOG_SIZE")) * 1024 * 1024;
	no_log = TypeConvert::StringToBoolean(LRCS_ENV::getGlobalConf("NO_LOG"));

	if (no_log)return;

	initLogSlot();
	logstr = new char[MAXLINSIZE + 1];

	InitializeCriticalSection(&csLock);

	logStream = NULL;

	if (log_2_file) {
		if(!openNewLogFile())
			throw new UnexpectedException("Log.cpp: Open log file failed!");
	}
	else {
		logStream = &cout;
	}
}

void Log::logDestroy() {
	if (log_2_file && logStream != NULL) {
		delete logStream;
		delete logSlot;
	}
	delete[] logstr;
}

bool Log::writeToLog(char* src_, int priority_, string msg_) {
	return writeToLog(src_, priority_, (char*)msg_.c_str());
}
bool Log::writeToLog(char* src_, int priority_, char* msg_, ...) {
	if (priority_ >= threshold && !no_log){
		va_list argp;
		if (msg_ == NULL || msg_[0] == 0)return false;
		string timeStamp = SysTime::getCurrTime();

		EnterCriticalSection(&csLock);
		*logStream << timeStamp << ":" << src_ << " MSG: ";
		va_start(argp, msg_);
		_vsnprintf(logstr, MAXLINSIZE, msg_, argp);
		*logStream << logstr;
		va_end(argp);
		*logStream << endl;
		if (logStream->tellp() > logSize){
			openNewLogFile();
		}
		else{
			((ofstream*)logStream)->flush();
		}
		LeaveCriticalSection(&csLock);
		//writeLogStream(src_, msg_);
	}	
	return true;
}

bool Log::writeToLog(char* src_, int priority_, char* msg_, int val_) {
	/*if (priority_ >= threshold && !no_log) {
		if (msg_ == NULL || msg_[0] == 0)return false;
		char* msgBuff = new char[512];
		sprintf(msgBuff, "%s%d", msg_, val_);
		writeLogStream(src_, msgBuff);
		delete[] msgBuff;
	}
			
	return true;*/
	return writeToLog(src_, priority_, "%s%d", msg_, val_);
}

void Log::writeLogStream(char* src_, char* msg_) {
	string timeStamp = SysTime::getCurrTime();
	EnterCriticalSection(&csLock);
	*logStream << timeStamp << ":" << src_ << " MSG: " << msg_ << endl;	
	if (logStream->tellp() > logSize){
		openNewLogFile();
	}
	else{
		((ofstream*)logStream)->flush();
	}
	LeaveCriticalSection(&csLock);
}

int Log::getCurrLogIndex(){
	map<int, string>::iterator m_it;
	for (m_it = logSlot->begin(); m_it != logSlot->end(); m_it++){
		if (strcmp(m_it->second.c_str(), "0") == 0)
			return m_it->first;
	}
	/*If all the slots are used,reset all the slot to initial*/
	for (m_it = logSlot->begin(); m_it != logSlot->end(); m_it++)
		m_it->second = "0";
	return 1;
}

void Log::setCurrLogIndex(int slot_){
	logSlot->at(slot_) = "1"; //Should always return the right value!!
	//logSlot[slot_] = "1";
	ofstream outstream;
	map<int, string>::iterator m_it;
	outstream.open(LOG_SLOT);
	for (m_it = logSlot->begin(); m_it != logSlot->end(); m_it++)
		outstream << m_it->first << SEPARATOR << m_it->second << endl;
	outstream.close();
}

bool Log::openNewLogFile(){
	if (logStream != NULL){
		((ofstream*)logStream)->close();
		char* buff = new char[256];
		int currLogSlot = getCurrLogIndex();
		sprintf(buff, "%s.%d", logFileName.c_str(), currLogSlot);
		string archiveLogName = buff;
		delete[] buff;
		if (rename(logFileName.c_str(), archiveLogName.c_str())){
			remove(archiveLogName.c_str());
			rename(logFileName.c_str(), archiveLogName.c_str());
		}
		setCurrLogIndex(currLogSlot);
	}
	else{
		logStream = new ofstream();
	}
	((ofstream*)logStream)->open(logFileName, ios::out | ios::app);
	return ((ofstream*)logStream)->is_open();
}

void Log::initLogSlot(){
	logSlot = new map<int, string>;
	ifstream* infile = new ifstream(LOG_SLOT);
	if (!infile->is_open()){
		delete infile;
		return;
	}
	string line;
	string whitespace = "\t ";
	while (getline((*infile), line)){
		line = StringUtil::ltrim(line, whitespace);
		string slotName = StringUtil::splitDelimitedSubstring(SEPARATOR, line, 1);
		string slotStatus = StringUtil::splitDelimitedSubstring(SEPARATOR, line, 2);
		int slot = TypeConvert::StringToInt(slotName);
		//logSlot[slot] = slotStatus;
		logSlot->insert(map<int, string>::value_type(slot, slotStatus));
	}
	delete infile;
}