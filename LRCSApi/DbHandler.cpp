#include "stdafx.h"
#include "DbHandler.h"


DbHandler::DbHandler()
{
}


DbHandler::~DbHandler()
{
	for (DataSourceMap::iterator it = m_DataSources.begin(); it != m_DataSources.end(); it++)
	{
		if (it->second.first.first != NULL)
			delete it->second.first.first;

		if (it->second.second != NULL)
			delete it->second.second;
	}
}
