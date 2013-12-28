#include "SysDbEnv.h"
#include "Constants.h"
#include "AccessException.h"

using namespace NS_SYSDBENV;

SysDbEnv::SysDbEnv()
{
}


SysDbEnv::~SysDbEnv()
{
}

void SysDbEnv::initDbEnv(){
	if (sysdbenv == NULL){
		try{
			sysdbenv = new DbEnv(0);
			sysdbenv->set_errpfx("SysDbEnv");
			FILE* errfile = fopen("SysDbEnvErr.log", "a");;
			sysdbenv->set_errfile(errfile);
			sysdbenv->set_error_stream(&cout);
			// Open the environment with full transactional supportDB_THREAD
			sysdbenv->open(SYSTEM_DB, DB_CREATE | DB_INIT_LOCK | DB_INIT_LOG | 
				DB_INIT_MPOOL | DB_INIT_TXN | DB_THREAD | DB_RECOVER, 0);
		}
		catch (DbException &dbe){
			throw new AccessException(dbe.what());
		}
		//strcpy(SysDbEnv::home, home);
	}
}

DbEnv* SysDbEnv::getDbEnv(){
	return sysdbenv;
}

void SysDbEnv::closeDbEnv(){
	try{
		sysdbenv->close(0);
	}
	catch (DbException &dbe){
		throw new AccessException(dbe.what());
	}
}
void SysDbEnv::teardown(const char* home){
	try{
		if (sysdbenv == NULL) sysdbenv = new DbEnv(0);
		sysdbenv->set_errpfx("SysDbEnv");
		sysdbenv->remove(home, 0);
	}
	catch (DbException &dbe){
		throw new AccessException(dbe.what());
	}
}