#include "stdafx.h"
#include "QueryResult.h"

QueryResult::QueryResult(int nErrorCode)
: m_nResultCode(nErrorCode)
{

}
QueryResult::QueryResult()
{
	m_nResultCode = Success;
}


QueryResult::~QueryResult()
{
	Destroy();
}

int QueryResult::ResultCode()
{
	return m_nResultCode;
}
int QueryResult::ResultCode(int nResultCode)
{
	m_nResultCode = nResultCode;
	return m_nResultCode;
}

QueryResult::QueryResult(const QueryResult& qr)
{
	this->operator=(qr);
}
QueryResult* QueryResult::operator= (const QueryResult& qr)
{
	if (this != &qr)
	{
		Destroy();

		m_nResultCode = qr.m_nResultCode;

		m_aryData = qr.m_aryData;
	}

	return this;
}

void QueryResult::Destroy()
{
	if (m_aryData.size() > 0)
	{
		m_aryData.clear();
	}
	m_nResultCode = 0;
}
