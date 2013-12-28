#include "BDBEnv.h"
#include "Constants.h"
#include "AccessException.h"

BDBEnv::BDBEnv()
{
}


BDBEnv::~BDBEnv()
{
}

void BDBEnv::initDbEnv(const char* home){
	// Create an environment object and initialize it for error
	// reporting.
	if (dbenv == NULL){
		try{
			dbenv = new DbEnv(0);
			dbenv->set_errpfx("BDBEnv");
			// Open the environment with full transactional support.
			dbenv->open(home, DB_CREATE | DB_INIT_LOCK | DB_INIT_LOG | DB_INIT_MPOOL | DB_INIT_TXN | DB_THREAD, 0);
		}
		catch (DbException &dbe){
			char *errbuf = new char[strlen(dbe.what()) + 1];
			strcpy(errbuf, dbe.what());
			throw new AccessException(errbuf);
		}
	}
}

DbEnv* BDBEnv::getDbEnv( ){
	return dbenv;
}

void BDBEnv::closeDbEnv(){
	dbenv->close(0);
	//delete dbenv; "Static Ptr can not be deleted, system will recycle the resource
}

void BDBEnv::teardown(const char* home){
	try{
		if (dbenv == NULL) dbenv = new DbEnv(0);
		dbenv->set_errpfx("BDBEnv");
		dbenv->remove(home, 0);
	}
	catch (DbException &dbe){
		char *errbuf = new char[strlen(dbe.what()) + 1];
		strcpy(errbuf, dbe.what());
		throw new AccessException(errbuf);
	}
}