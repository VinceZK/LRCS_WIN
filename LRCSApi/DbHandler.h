#pragma once

#include <vector>
#include <map>
#include <utility>


class ROSAM;
class DataSource;

class DbHandler
{
public:
	// DataLen, Column Count
	typedef std::pair<int, int>			ColumnInfo;
	typedef std::pair<DataSource*, ColumnInfo>	DataSourceInfo;
	typedef std::pair<DataSourceInfo, ROSAM*>	DataSourcePair;
	typedef std::map<tstring, DataSourcePair> DataSourceMap;

	DbHandler();
	~DbHandler();

	void SetDataSource(LPCTSTR szTable, ROSAM* pRosAM, DataSource* pDS, int nDataLen, int nColumnCount = 1){
		DataSourceMap::iterator it = m_DataSources.find(szTable);
		if (it != m_DataSources.end())
			return;

		m_DataSources[szTable] = std::make_pair(std::make_pair(pDS, std::make_pair(nDataLen, nColumnCount)), pRosAM);
	}
	
	std::vector<DataSourceInfo> GetDataSources()
	{
		std::vector<DataSourceInfo> result;
		for (DataSourceMap::iterator it = m_DataSources.begin(); it != m_DataSources.end(); ++it)
		{
			result.push_back(it->second.first);
		}
		return result;
	}	
	
	DataSourceInfo	GetDataSource(LPCTSTR szTable)
	{
		DataSourceMap::iterator it = m_DataSources.find(szTable);
		if (it != m_DataSources.end())
		{
			return it->second.first;
		}

		return DataSourceInfo(NULL, std::make_pair(0, 0));
	}

	int TotalColumn()
	{
		int nTotal = 0;
		for (DataSourceMap::iterator it = m_DataSources.begin(); it != m_DataSources.end(); ++it)
		{
			nTotal += it->second.first.second.second;
		}

		return nTotal;
	}

private:
	DataSourceMap m_DataSources;
};

