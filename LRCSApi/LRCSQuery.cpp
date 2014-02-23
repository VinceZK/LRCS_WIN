#include "stdafx.h"

#include "LRCSQuery.h"
#include "tchar.h"
#include "AtlBase.h"
#include "AtlConv.h"
#include "../LRCSBasis/ROSAM.h"
#include "../LRCSBasis/ProjMaker.h"
#include "../LRCSBasis/LRCS_ENV.h"
#include "../LRCSBasis/DataSource.h"
#include "../LRCSBasis/StringDataSource.h"
#include "../LRCSBasis/LZDataSource.h"
#include "../LRCSBasis/BitmapDataSource.h"
#include "../LRCSBasis/RLEDataSource.h"
#include "..\LRCSBasis\Predicate.h"
#include "DbHandler.h"
#include "LRCSQueryImpl.h"
LRCSQuery::LRCSQuery()
{
	m_bInited = false;
}


LRCSQuery::~LRCSQuery()
{
	Destroy();
}

bool LRCSQuery::Init()
{
	if (m_bInited)
	{
		Destroy();
	}

	m_bInited = LRCS_ENV::initLRCS();

	return m_bInited;
}

void LRCSQuery::Destroy()
{
	if (m_bInited)
	{
		for (size_t i = 0; i < m_dbHandlers.size(); i++)
		{
			if (m_dbHandlers[i] != NULL)
			{
				delete m_dbHandlers[i];
				m_dbHandlers[i] = NULL;
			}
		}

		m_dbHandlers.clear();
		m_dbTableFiles.clear();

		LRCS_ENV::closeLRCS();

		m_bInited = false;
	}
}

int LRCSQuery::OpenDbTableByTable(LPCTSTR szTablePath)
{
	return OpenDbTableByProj(GetProjName(szTablePath).c_str());	
}

int LRCSQuery::OpenDbTableByProj(LPCTSTR szProjName)
{
	USES_CONVERSION;

	if (!m_bInited)
	{
		if (!Init())
		{
			return Fail_Init_LRCS_Environment;
		}
	}

	for (unsigned int i = 0; i < m_dbTableFiles.size(); ++i)
	{
		if (m_dbTableFiles[i] == szProjName)
			// This File has been opened.
			return i;	// return handle of the project.
	}

	auto_ptr<ProjMaker> spProjMaker(new ProjMaker());

	char projName[MAX_NAME_LENGTH];
	strncpy(projName, CT2A(szProjName), MAX_NAME_LENGTH);
	Projection* pProj = spProjMaker->getProjection(projName);

	if (pProj == NULL)
		return Table_No_Project;	// No proj file;

	vector<COLUMN_REC>* columnsPtr = pProj->getProjColumns();
	if (columnsPtr->size() == 0)
		return Invalid_Project_NoTable;	// No column setting.

	DbHandler* pDb = new DbHandler();

	for (vector<COLUMN_REC>::iterator it = columnsPtr->begin(); it != columnsPtr->end(); ++it)
	{
		ROSAM* pAM = new ROSAM(it->columnName, 2, sizeof(int), it->dataLength, ValPos::INTTYPE, it->dataType);

		DataSource* pDS = NULL;
		switch (it->compressionType)
		{
		case LZO_COMPRESSION:
			pDS = new LZDataSource(pAM, it->sorted, true, new StringDecoder(it->sorted));
			break;
		case BITMAP_COMPRESSION:
			pDS = new BitmapDataSource(pAM, it->sorted, true, new StringDecoder(it->sorted));
			break;
		case RLE_COMPRESSION: 
			pDS = new RLEDataSource(pAM, it->sorted, true, new StringDecoder(it->sorted));
			break;
		case NO_COMPRESSION:
		default:
			//if (it->dataType == ValPos::STRINGTYPE)
			{
				pDS = new StringDataSource(pAM, it->sorted, true);
			}
			break;
		}

		pDb->SetDataSource(CA2T(it->columnName), pAM, pDS, it->dataLength);
	}

	m_dbHandlers.push_back(pDb);
	m_dbTableFiles.push_back(szProjName);

	return m_dbTableFiles.size() - 1;
}

void LRCSQuery::CloseDbTable(int dbTableHandle)
{
	if (IsValidDbHandle(dbTableHandle))
	{
		delete m_dbHandlers[dbTableHandle];
		m_dbHandlers[dbTableHandle] = NULL;

		m_dbTableFiles[dbTableHandle] = _T("");
	}
}

bool LRCSQuery::IsValidDbHandle(int dbTableHandle)
{
	return m_dbTableFiles.size() > dbTableHandle && !m_dbTableFiles[dbTableHandle].empty();
}

QueryResult LRCSQuery::QueryData(QueryParam param)
{
	if (!IsValidDbHandle(param.nDbTableHandle))
		return QueryResult(Invalid_Parameter);

	if (param.szQueryDatas.size() != param.szTableNames.size() || param.ePredicateTypes.size() != param.szQueryDatas.size())
		return QueryResult(Invalid_Parameter);

	std::vector<std::pair<Predicate*, ValPos*> > queryData;

	for (int i = 0; i < param.szTableNames.size(); ++i)
	{
		DbHandler::DataSourceInfo dsp = m_dbHandlers[param.nDbTableHandle]->GetDataSource(param.szTableNames[i]);
		if (dsp.first == NULL)
			return QueryResult(Invalid_Parameter);

		Predicate* pPred = new Predicate((Predicate::PredicateType)param.ePredicateTypes[i]);
		dsp.first->setPredicate(pPred);

		ValPos* queryValue = new StringValPos(dsp.second.first);
		char* pszValue = new char[dsp.second.first + 1];
		memset(pszValue, 0, dsp.second.first + 1);
		strncpy_s(pszValue, dsp.second.first + 1, CT2A(param.szQueryDatas[i]), dsp.second.first);
		queryValue->set((byte*)pszValue);

		dsp.first->changeRHSBinding(queryValue);

		queryData.push_back(std::make_pair(pPred, queryValue));

		delete[] pszValue;
	}

	QueryResult result = LRCSQueryImpl().DoQuery(m_dbHandlers[param.nDbTableHandle]);

	for (int i = 0; i < queryData.size(); ++i)
	{
		delete queryData[i].first;
		delete queryData[i].second;
	}

	return result;
}

tstring	LRCSQuery::GetProjName(LPCTSTR szTable)
{
	tstring strProj = szTable;
	strProj += _T("proj");

	return strProj;
}

