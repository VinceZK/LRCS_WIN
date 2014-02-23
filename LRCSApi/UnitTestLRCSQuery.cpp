#include "stdafx.h"

#ifdef USE_GTEST
#include "UnitTestLRCSQuery.h"
#include "tchar.h"

TestLRCSQuery::TestLRCSQuery()
{
}


TestLRCSQuery::~TestLRCSQuery()
{
}

TEST_F(TestLRCSQuery, QueryData)
{
	int nDbHandler = query.OpenDbTableByTable(_T("StringTest01"));
	EXPECT_GE(nDbHandler, 0);

	if (nDbHandler < 0)
		return;

	LRCSQuery::QueryParam param;

	param.nDbTableHandle = nDbHandler;
	param.szTableNames.push_back(_T("StringTest01.ID"));
	param.szQueryDatas.push_back(_T("abcde99990"));
	param.ePredicateTypes.push_back(LRCSQuery::QueryParam::OP_GREATER_THAN_OR_EQUAL);

	param.szTableNames.push_back(_T("StringTest01.DESC"));
	param.szQueryDatas.push_back(_T("Cstore00049"));
	param.ePredicateTypes.push_back(LRCSQuery::QueryParam::OP_LESS_THAN);

	DWORD dwBeginQuery = ::GetTickCount();
	QueryResult result = query.QueryData(param);

	std::cout << "QueryData RunTime: " << (::GetTickCount() - dwBeginQuery) / 1000 << "ms" << std::endl;
	EXPECT_EQ(result.ResultCode(), Success);
	EXPECT_GT(result.RowCount(), 0);
	EXPECT_GT(result.ColCount(), 0);

	for (int i = 0; i < result.RowCount(); ++i)
	{
		for (int j = 0; j < result.ColCount(); ++j)
		{
			LRCSValue value = result.GetValue(i, j);
			switch (value.ValueType())
			{
			case LRCSValue::INTTYPE:
				std::cout << value.GetValue<int>();
				break;
			case LRCSValue::FLOATTYPE:
				break;
				std::cout << value.GetValue<float>();
			case LRCSValue::DOUBLETYPE:
				std::cout << value.GetValue<double>();
				break;
			case LRCSValue::LONGTYPE:
				std::cout << value.GetValue<long>();
				break;
			case LRCSValue::STRINGTYPE:
#ifdef UNICODE
				std::wcout << value.GetValue<tstring>().c_str();
#else
				std::cout << value.GetValue<tstring>().c_str();
#endif
				break;
			default:
				break;
			}

			if (j != result.ColCount() - 1)
#ifdef UNICODE
				std::wcout << _T(",");
#else
				std::cout << _T(",");
#endif
		}

		std::cout << std::endl;
	}

	query.CloseDbTable(nDbHandler);
}

#endif