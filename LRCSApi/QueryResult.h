#pragma once

#include "LRCSValue.h"
#include <vector>

enum ErrorCode
{
	Success = 0, 	// Success
	Invalid_Parameter	= -0x10001,	//Invalid parameters
	Invalid_Project_NoTable = -0x10002,		//There isn't table record in the project

	Table_No_Project = -0x20001,		//Cannot find the project of the table in database.
	Fail_Init_LRCS_Environment = -0x30001,	// Fail to run the LRCS_ENV::initLRCS()
};

class LRCSAPI_API QueryResult
{
public:
	typedef std::vector<LRCSValue>		QueryResultRow;
	typedef std::vector<QueryResultRow>	QueryResultTable;
	QueryResult(int nErrorCode);
	QueryResult();
	~QueryResult();

	QueryResult(const QueryResult& qr);
	QueryResult* operator= (const QueryResult& qr);
public:
	// Query result code
	//		0: success
	inline int ResultCode();
	inline int ResultCode(int nResultCode);
	// Message of the result code.
	//		0: success
	inline LPCTSTR  ResultMessage();

	// QueryResult is an two-dimension array. It just likes the db table.
	//		Total row number of the table.
	int  RowCount(){ return m_aryData.size(); }
	//		Total column number of the table.
	int ColCount(){
		if (RowCount() > 0)
			return m_aryData[0].size();

		return 0;
	}

	// The query result is a table.
	//    Row
	//	  Column
	QueryResultTable GetTable(){ return m_aryData; }

	// Query result
	//	  Row
	QueryResultRow GetRow(int nRowIndex){
		return m_aryData[nRowIndex];
	}

	// Query Result
	//	  Field
	LRCSValue GetValue(int nRowIndex, int nColIndex){
		return m_aryData[nRowIndex][nColIndex];
	}

	// Add a row of the result
	// Return
	//	Row index in the Table.
	int AddRow(QueryResultRow& row)
	{
		m_aryData.push_back(row);

		return m_aryData.size() - 1;
	}
private:
	void Destroy();

	int m_nResultCode;
	QueryResultTable m_aryData;

};

