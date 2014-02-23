#pragma once

#include "QueryResult.h"
#include <vector>

#ifdef USE_GTEST
#include <gtest\gtest.h>
#endif

class ROSAM;
class DbHandler;

class LRCSAPI_API LRCSQuery
{
public:
#ifdef USE_GTEST
	FRIEND_TEST(TestLRCSQuery, QueryData);
#endif

	LRCSQuery();
	~LRCSQuery();

public:
	// Initialize the query objest. Must be called firstly.
	bool Init();

	// Destroy the resources.
	void Destroy();

	// Open a table(file) to query by the default project name.
	// return 
	//		The handle of the table. We need this handle to query, close and so on.
	//		-1 : fail to open the file(table)
	//		>= 0: success to open the file(table)
	int OpenDbTableByTable(LPCTSTR szTablePath);

	// Open the table with the project name
	int OpenDbTableByProj(LPCTSTR szProjName);

	// Close the DbTable.
	void CloseDbTable(int dbTableHandle);

	inline bool IsValidDbHandle(int dbTableHandle);

	struct QueryParam
	{
		enum PredicateType {
			OP_LESS_THAN,
			OP_LESS_THAN_OR_EQUAL,
			OP_EQUAL,
			OP_GREATER_THAN_OR_EQUAL,
			OP_GREATER_THAN,
			//OP_EVEN,
			//OP_ODD	
		};
		int nDbTableHandle;

		std::vector<LPCTSTR>	szTableNames;
		std::vector<LPCTSTR>	szQueryDatas;
		std::vector<PredicateType> ePredicateTypes;
	};
	QueryResult QueryData(QueryParam param);
private:
	tstring	GetProjName(LPCTSTR szTable);

	bool m_bInited;

	std::vector<tstring>	m_dbTableFiles;
	std::vector<DbHandler*>	m_dbHandlers;
};

