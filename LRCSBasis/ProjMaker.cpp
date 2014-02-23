#include "ProjMaker.h"
#include "Constants.h"
#include "SysDbEnv.h"
#include "AccessException.h"
#include "CompareRoutine.h"
#include "GUIDGen.h"
#include "SysTime.h"

ProjMaker::ProjMaker()
{
	sysdbenv = SysDbEnv::getDbEnv();
	initialized = false;
	sysProjection = NULL;
	sysColumn = NULL;
	sysColumnSecondary = NULL;
}


ProjMaker::~ProjMaker()
{
	closeDbStruc();
}

void ProjMaker::initDbStruc(DbTxn* tid){
	try{
		sysProjection = new Db(sysdbenv, 0);
		sysProjection->set_pagesize(4 * 1024);
		sysProjection->set_bt_compare(CompareRoutine::compare_key);
		sysProjection->open(tid, "sysdb", SYS_PROJECTION, DB_BTREE, DB_CREATE, 0664);

		sysColumnSecondary = new Db(sysdbenv, 0);
		sysColumnSecondary->set_pagesize(4 * 1024);
		sysColumnSecondary->set_flags(DB_DUP | DB_DUPSORT);
		sysColumnSecondary->set_bt_compare(CompareRoutine::compare_key);
		sysColumnSecondary->open(tid, "sysdb", SYS_COLUMN_SECONDARY, DB_BTREE, DB_CREATE, 0664);

		sysColumn = new Db(sysdbenv, 0);
		sysColumn->set_pagesize(4 * 1024);
		sysColumn->set_bt_compare(CompareRoutine::compare_key);
		sysColumn->open(tid, "sysdb", SYS_COLUMN, DB_BTREE, DB_CREATE, 0664);
		/* Associate the secondary with the primary. */
		sysColumn->associate(tid, sysColumnSecondary, getProjName, 0);
	}
	catch (DbException &dbe){
		throw new AccessException(dbe.what());
	}

	initialized = true;
}

void ProjMaker::closeDbStruc(){
	if (sysProjection != NULL){
		sysProjection->close(0);
		delete sysProjection;
	}

	if (sysColumn != NULL){
		sysColumn->close(0);
		delete sysColumn;
	}
	if (sysColumnSecondary != NULL) {
		sysColumnSecondary->close(0);
		delete sysColumnSecondary;
	}
}

void ProjMaker::createProjection(Projection* proj_){
	//DbTxn* tid;
	Dbt dkey, ddata;
	int ret;

	PROJECTION_REC*  projRec = proj_->getProjectionRec();
	vector<COLUMN_REC>* columns = proj_->getProjColumns();

	sysdbenv->txn_begin(NULL, &tid, 0);
	if (!initialized) initDbStruc(tid);
	//insert  $sys$projection table
	memset(&dkey, 0, sizeof(dkey));
	memset(&ddata, 0, sizeof(ddata));

	dkey.set_data(projRec->projectionName);
	dkey.set_size(sizeof(projRec->projectionName));
	dkey.set_ulen(sizeof(projRec->projectionName));
	dkey.set_flags(DB_DBT_USERMEM);

	strcpy_s(projRec->createTime, SysTime::getCurrTime().c_str());
	strcpy_s(projRec->alterTime, SysTime::getCurrTime().c_str());
	ddata.set_data(projRec);
	ddata.set_size(sizeof(*projRec));
	ddata.set_ulen(sizeof(*projRec));
	ddata.set_flags(DB_DBT_USERMEM);

	ret = sysProjection->put(tid, &dkey, &ddata, DB_NOOVERWRITE);
	if (ret != 0){
		tid->abort();
		throw new AccessException("ProjMaker: $SYS$Projection createProjection failed!");
	}

	//insert  $sys$column table
	for (int i = 0; i < columns->size(); i++){
		memset(&dkey, 0, sizeof(dkey));
		memset(&ddata, 0, sizeof(ddata));
		strcpy_s(columns->at(i).columnID, GUIDGen::getGUID().c_str());
		strcpy_s(columns->at(i).createTime, SysTime::getCurrTime().c_str());
		dkey.set_data(&(columns->at(i).columnID));
		dkey.set_size(GUID_LENGTH);
		dkey.set_ulen(GUID_LENGTH);
		dkey.set_flags(DB_DBT_USERMEM);

		ddata.set_data(&(columns->at(i)));
		ddata.set_size(sizeof(columns->at(i)));
		ddata.set_ulen(sizeof(columns->at(i)));
		ddata.set_flags(DB_DBT_USERMEM);
		ret == sysColumn->put(tid, &dkey, &ddata, 0);
		if (ret != 0){
			tid->abort();
			throw new AccessException("ProjMaker: $SYS$Column createProjection failed!");
		}
	}
	tid->commit(0);

}


Projection* ProjMaker::getProjection(char* projName_){
	Dbt key, data, pkey;
	Dbc* iter;
	int ret;
	PROJECTION_REC* projRec;
	COLUMN_REC* colRec;
	vector<COLUMN_REC> columns;
	Projection* proj = new Projection(projName_);
	sysdbenv->txn_begin(NULL, &tid, 0);
	if (!initialized) initDbStruc(tid);
	//get  $sys$projection table
	sysProjection->cursor(tid, &iter, 0);
	memset(&key, 0, sizeof(key));
	memset(&data, 0, sizeof(data));
	key.set_flags(DB_DBT_USERMEM);
	key.set_ulen(MAX_NAME_LENGTH);
	key.set_size(MAX_NAME_LENGTH);
	key.set_data(projName_);

	data.set_flags(DB_DBT_MALLOC);
	ret = iter->get(&key, &data, DB_SET);
	switch (ret = iter->get(&key, &data, DB_SET)){
	case 0:
		projRec = (PROJECTION_REC*)data.get_data();
		proj->setProjectionRec(projRec);
		iter->close();
		break;
	case DB_NOTFOUND:
		delete proj;
		return NULL;
	default:
		throw new AccessException("ProjMaker : getProjection failed!");
	}

	//get columns belongs to the projection
	sysColumnSecondary->cursor(tid, &iter, 0);
	memset(&key, 0, sizeof(key));
	memset(&data, 0, sizeof(data));
	memset(&pkey, 0, sizeof(pkey));
	key.set_flags(DB_DBT_USERMEM);
	key.set_ulen(MAX_NAME_LENGTH);
	key.set_size(MAX_NAME_LENGTH);
	key.set_data(projName_);
	data.set_flags(DB_DBT_MALLOC);
	if (ret = iter->pget(&key, &pkey, &data, DB_SET) == 0){
		colRec = (COLUMN_REC*)data.get_data();
		columns.push_back(*colRec);
		while (ret = iter->pget(&key, &pkey, &data, DB_NEXT_DUP) == 0){
			colRec = (COLUMN_REC*)data.get_data();
			columns.push_back(*colRec);
		}
	}
	proj->setProjColumns(&columns);

	iter->close();
	tid->commit(0);

	return proj;
}

int ProjMaker::getProjName(Db* pDb_, const Dbt* pkey_, const Dbt* data_, Dbt* skey_){
	memset(skey_, 0, sizeof(DBT));
	skey_->set_data(((COLUMN_REC*)data_->get_data())->projectionName);
	skey_->set_size(sizeof(((COLUMN_REC *)data_->get_data())->projectionName));
	return (0);
}