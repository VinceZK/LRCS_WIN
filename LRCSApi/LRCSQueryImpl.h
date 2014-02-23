#pragma once

#include "QueryResult.h"

class DbHandler;
class DataSource;
class Block;

class LRCSQueryImpl
{
public:
	LRCSQueryImpl();
	~LRCSQueryImpl();

public:
	QueryResult	DoQuery(DbHandler* pDbHandler);

private:
	bool FillDataToQueryResult(DataSource* dataSrc_, int currCol, int currTotalCol, Block** pBlks, LRCSValue& value);

	static int MULTITHREAD_WAY;
};

