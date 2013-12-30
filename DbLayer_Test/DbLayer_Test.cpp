// DbLayer_Test.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "DbLayer_Test.h"

#include <db_cxx.h>
#include "AtlBase.h"
#include "AtlConv.h"

#include "DataDecoder.h"

// This is an example of an exported variable
DBLAYER_TEST_API int nDbLayer_Test=0;

// This is an example of an exported function.
DBLAYER_TEST_API int fnDbLayer_Test(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see DbLayer_Test.h for the class definition
CDbLayer_Test::CDbLayer_Test()
	: m_bInit(false)
{	
}

CDbLayer_Test::~CDbLayer_Test()
{
	DestroyDb();
}

bool CDbLayer_Test::InitDb(LPCTSTR szDbFile)
{
	USES_CONVERSION;
	if (m_bInit)
	{
		DestroyDb();
	}
	m_spDb.reset(new Db(NULL, 0));

	int nRet = m_spDb->open(NULL, T2CA(szDbFile), NULL, DB_BTREE, DB_RDONLY, 0664);
	if (nRet != 0)
	{
		_ASSERT(_T("Fail to open database file!"));
		return false;
	}
	
	if (0 != m_spDb->cursor(NULL, &m_pCursor, 0))
	{
		m_spDb->close(0);
		m_spDb.reset();
		_ASSERT(_T("Fail to create cursor!"));
		return false;
	}

	m_strDbFile = szDbFile;
	m_bInit = true;

	return true;;
}

void CDbLayer_Test::DestroyDb()
{
	if (m_bInit)
	{
		m_pCursor->close();
		m_spDb->close(0);
		m_spDb.reset();
		m_bInit = false;
		m_pCursor = NULL;
	}
}

std::vector<tstring> CDbLayer_Test::GetFirstData()
{
	std::vector<tstring> result;
	if (m_bInit)
	{
		Dbt key, data;
		memset(&key, 0, sizeof(Dbt));
		memset(&data, 0, sizeof(Dbt));
		if(0 == m_pCursor->get(&key, &data, DB_FIRST))
		{
			result.push_back(_T("key:") + DataDecoder().ToString((BYTE*)key.get_data(), key.get_size(), DataDecoder::eDTString));
			result.push_back(_T("value:") + DataDecoder().ToString((BYTE*)data.get_data(), data.get_size(), data.get_size() == 4 ? DataDecoder::eDTInt : DataDecoder::eDTString));
		}
	}

	return result;
}
//
//tstring CDbLayer_Test::ToTString(char* psz, int nLen)
//{
//	USES_CONVERSION;
//	if (nLen == 4)
//	{
//		TCHAR pBuf[1024];
//		memset(pBuf, 0, 1024);
//		_stprintf_s(pBuf, 1024, _T("%d"), *((int*)psz));
//		return tstring(pBuf);
//	}
//	else
//	{
//		char* pBuf = new char[nLen+1];
//		memset(pBuf, 0, nLen+1);
//		strncpy_s(pBuf, nLen+1, psz, nLen);
//		return tstring(A2CT(pBuf));
//	}
//}

std::vector<tstring> CDbLayer_Test::GetNextData()
{
	std::vector<tstring> result;
	Dbt key, data;
	memset(&key, 0, sizeof(Dbt));
	memset(&data, 0, sizeof(Dbt));
	if (0 == m_pCursor->get(&key, &data, DB_NEXT))
	{
		result.push_back(_T("key:") + DataDecoder().ToString((BYTE*)key.get_data(), key.get_size(), DataDecoder::eDTString));
		result.push_back(_T("value:") + DataDecoder().ToString((BYTE*)data.get_data(), data.get_size(), data.get_size() == 4 ? DataDecoder::eDTInt : DataDecoder::eDTString));
	}

	return result;
}
