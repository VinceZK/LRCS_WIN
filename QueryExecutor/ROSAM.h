#pragma once
#include <db_cxx.h>
#include <string>
#include <vector>
#include <map>
#include "AccessException.h"
#include "Constants.h"
#include "Util.h"
#include <errno.h>
#include "IntWriter.h"
#include "AM.h"

/**
* Access BDB columns
*/

using namespace std;
class __declspec(dllexport) ROSAM :
	public AM
{
public:
	ROSAM(string table, int numindicies, int primkeysize_, int seckeysize_, short primkeytype_, short seckeytype_);
	virtual ~ROSAM();

	const void* getNextPage(char* key, Dbc *iter, u_int32_t flags, bool primary, char* buffer);
	const void* getDbSet(char* rkey);
	const void* getDbSetRange(char* rkey);
	const void* getDbNext();
	const void* getDbNextRange(char* lowKey, char* highKey, bool firstCall_);
	const void* getDbNextRange(char* highKey, bool firstCall_);

	const void* getDbPageSet(char* rkey);
	const void* getDbPageRange(char* rkey);
	const void* getDbPageNext();
	const void* getDbPageNextDup();
	const void* getDbPageNextRange(char* lowKey, char* highKey, bool firstCall_);
	const void* getDbPageNextRange(char* highKey, bool firstCall_);

	virtual void initCursors();
	virtual const void* getNextPagePrimary();
	virtual const void* getNextPageSecondary();
	virtual const void* skipToPagePrimary(char* key);
	virtual const void* skipToPageSecondary(char* key);
	virtual char* getLastIndexValuePrimary();
	virtual char* getLastIndexValueSecondary();
	virtual string toString();
	virtual string getTableName();
	virtual string getColumnName(){ return _colName; }
	virtual int getNumIndices() { return _numindicies; }

	/*static int compare_int(Db *dbp, const Dbt *a, const Dbt *b);
	static int compare_key(Db *dbp, const Dbt *a, const Dbt *b);
	static int compare_double(Db *dbp, const Dbt *a, const Dbt *b);*/
	static const unsigned int BULK_BUFFER_SIZE = 4 * 1024 * 1024;

protected:

	Dbc *_iterator;
	string _tableName;
	vector<Db*> _db_arr;
	vector<Dbc*> _iter_arr;
	int _numindicies;
	string table;
	string _colName;

	char* _internalBuffer;
	char* _bulkBuffer;
	unsigned int _bulkBuffer_size;
	Dbt bulk_key, bulk_data;
	DbMultipleKeyDataIterator* bulk_iter;
	DbMultipleDataIterator* dBulk_iter;
	bool bulk_empty;

	int primkeysize;
	int seckeysize;
	int primkeytype;
	int seckeytype;
	byte* pos_array;

	void loadTable(string table);
	char* acquireInternalBuffer();
	char* acquireMoreBuffer();

private:
	const void* doCursorGetPos(char* key_, u_int32_t flags);
	const void* doCursorGetPosMulti(char* key_, u_int32_t flags);
	const void* doCursorGetPage(char* key, u_int32_t flags);
};

