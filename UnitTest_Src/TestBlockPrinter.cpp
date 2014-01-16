#include "stdafx.h"
#include "..\LRCSBasis\BlockPrinter.h"

// Please DON'T write the code out of the "USE_GTEST" block.

#ifdef USE_GTEST
// Please write the test code under this block.

#include "..\LRCSBasis\LZDataSource.h"
#include "..\LRCSBasis\RLEDataSource.h"
#include "..\LRCSBasis\StringDataSource.h"
#include "..\LRCSBasis\BitmapDataSource.h"

class TestBlockPrinter : public testing::Test
{
protected:
	void SetUp(){
		m_dwBeginTime = GetTickCount();

	}
	void TearDown() {
		m_dwEndTime = GetTickCount();

		//EXPECT_TRUE(m_dwEndTime - m_dwBeginTime < 100) << "The performance is too low.";
	}

private:
	DWORD m_dwBeginTime;
	DWORD m_dwEndTime;
};

#define TestCase1_Query1S(fileName1, fileName2, queryWay, aryQueryResult, queryValue1, queryPred1, queryValue2, queryPred2)				\
{																													\
	auto_ptr<ROSAM> am1(new ROSAM(fileName1, 2, sizeof(int), 10, ValPos::INTTYPE, ValPos::STRINGTYPE));				\
	Log::writeToLog("Query1S", 10, "Opening Column2...");															\
	auto_ptr<ROSAM> am2(new ROSAM(fileName2, 2, sizeof(int), 11, ValPos::INTTYPE, ValPos::STRINGTYPE));				\
	auto_ptr<Predicate> pred1(new Predicate(queryPred1)); 															\
	auto_ptr<Predicate> pred2(new Predicate(queryPred2)); 															\
	auto_ptr<StringDataSource> ds1(new StringDataSource(am1.get(), true, true));									\
	auto_ptr<StringDataSource> ds2(new StringDataSource(am2.get(), false, true));									\
																													\
	ds1->setPredicate(pred1.get());																					\
	ds2->setPredicate(pred2.get());																					\
																													\
	ValPos* rhs1 = new StringValPos(10);																			\
	char* RHSVal1 = queryValue1;																					\
	rhs1->set((byte*)RHSVal1);																						\
	ds1->changeRHSBinding(rhs1);																					\
																													\
	ValPos* rhs2 = new StringValPos(11);																			\
	char* RHSVal2 = queryValue2;																					\
	rhs2->set((byte*)RHSVal2);																						\
	ds2->changeRHSBinding(rhs2);																					\
																													\
	DataSource* srcs[2] = { ds1.get(), ds2.get() };																	\
	int numCols[2] = { 1, 1 };																						\
																													\
	auto_ptr<BlockPrinter> bPrint(new BlockPrinter(srcs, numCols, 2, NULL));										\
																													\
	BlockPrinter::MULTITHREAD_WAY = queryWay;																		\
	bPrint->wrapperOfPrintColumns(true, false);																		\
																													\
	aryQueryResult = bPrint->m_aryQueryResult;																		\
}

#define TestCase1_Query2S(fileName1, fileName2, queryWay, aryQueryResult, queryValue1, queryPred1, queryValue2, queryPred2)	\
{																													\
	auto_ptr<ROSAM> am1(new ROSAM(fileName1, 2, sizeof(int), 10, ValPos::INTTYPE, ValPos::STRINGTYPE));				\
	auto_ptr<ROSAM> am2(new ROSAM(fileName2, 2, sizeof(int), 11, ValPos::INTTYPE, ValPos::STRINGTYPE));				\
	auto_ptr<Predicate> pred1(new Predicate(queryPred1));															\
	auto_ptr<Predicate> pred2(new Predicate(queryPred2));															\
	auto_ptr<LZDataSource> ds1(new LZDataSource(am1.get(), true, true, (new StringDecoder(true))));					\
	auto_ptr<LZDataSource> ds2(new LZDataSource(am2.get(), true, true, (new StringDecoder(true))));					\
																													\
	ds1->setPredicate(pred1.get());																					\
	ds2->setPredicate(pred2.get());																					\
																													\
	auto_ptr<ValPos> rhs1(new StringValPos(10));																	\
	char* RHSVal1 = queryValue1;																					\
	rhs1->set((byte*)RHSVal1);																						\
	ds1->changeRHSBinding(rhs1.get());																				\
																													\
	auto_ptr<ValPos> rhs2(new StringValPos(11));																	\
	char* RHSVal2 = queryValue2;																					\
	rhs2->set((byte*)RHSVal2);																						\
	ds2->changeRHSBinding(rhs2.get());																				\
																													\
	DataSource* srcs[2] = { ds1.get(), ds2.get() };																	\
	int numCols[2] = { 1, 1 };																						\
																													\
																													\
	auto_ptr<BlockPrinter> bPrint(new BlockPrinter(srcs, numCols, 2, "Query2S.out"));								\
	BlockPrinter::MULTITHREAD_WAY = queryWay;																		\
	bPrint->wrapperOfPrintColumns(true, false);																		\
																													\
	aryQueryResult = bPrint->m_aryQueryResult;																		\
}

#define TestCase1_Query3S(fileName1, fileName2, queryWay, aryQueryResult, queryValue1, queryPred1, queryValue2, queryPred2)	\
{\
	auto_ptr<ROSAM> am1(new ROSAM(fileName1, 2, sizeof(int), 10, ValPos::INTTYPE, ValPos::STRINGTYPE));				\
	auto_ptr<ROSAM> am2(new ROSAM(fileName2, 2, sizeof(int), 4, ValPos::INTTYPE, ValPos::STRINGTYPE));				\
	auto_ptr<Predicate> pred1(new Predicate(queryPred1));															\
	auto_ptr<Predicate> pred2(new Predicate(queryPred2));															\
	auto_ptr<LZDataSource> ds1(new LZDataSource(am1.get(), true, true, (new StringDecoder(true))));					\
	auto_ptr<BitmapDataSource> ds2(new BitmapDataSource(am2.get(), false, true, (new StringDecoder(true))));				\
																													\
	ds1->setPredicate(pred1.get());																					\
	ds2->setPredicate(pred2.get());																					\
																													\
	auto_ptr<ValPos> rhs1(new StringValPos(10));																	\
	char* RHSVal1 = queryValue1;																					\
	rhs1->set((byte*)RHSVal1);																						\
	ds1->changeRHSBinding(rhs1.get());																				\
																													\
	auto_ptr<ValPos> rhs2(new StringValPos(4));																		\
	char* RHSVal2 = queryValue2;																					\
	rhs2->set((byte*)RHSVal2);																						\
	ds2->changeRHSBinding(rhs2.get());																				\
																													\
	DataSource* srcs[2] = { ds1.get(), ds2.get() };																	\
	int numCols[2] = { 1, 1 };																						\
																													\
																													\
	auto_ptr<BlockPrinter> bPrint(new BlockPrinter(srcs, numCols, 2, "Query3S.out"));								\
	BlockPrinter::MULTITHREAD_WAY = queryWay;																		\
	bPrint->wrapperOfPrintColumns(true, false);																		\
																													\
	aryQueryResult = bPrint->m_aryQueryResult;																		\
}

#define TestCase1_Query4S(fileName1, fileName2, queryWay, aryQueryResult, queryValue1, queryPred1, queryValue2, queryPred2)	\
{\
	auto_ptr<ROSAM> am1(new ROSAM(fileName1, 2, sizeof(int), 10, ValPos::INTTYPE, ValPos::STRINGTYPE));				\
	auto_ptr<ROSAM> am2(new ROSAM(fileName2, 2, sizeof(int), 4, ValPos::INTTYPE, ValPos::STRINGTYPE));				\
	auto_ptr<Predicate> pred1(new Predicate(queryPred1));															\
	auto_ptr<Predicate> pred2(new Predicate(queryPred2));															\
	auto_ptr<LZDataSource> ds1(new LZDataSource(am1.get(), true, true, (new StringDecoder(true))));					\
	auto_ptr<RLEDataSource> ds2(new RLEDataSource(am2.get(), false, true, (new StringDecoder(true))));				\
																													\
	ds1->setPredicate(pred1.get());																					\
	ds2->setPredicate(pred2.get());																					\
																													\
	auto_ptr<ValPos> rhs1(new StringValPos(10));																	\
	char* RHSVal1 = queryValue1;																					\
	rhs1->set((byte*)RHSVal1);																						\
	ds1->changeRHSBinding(rhs1.get());																				\
																													\
	auto_ptr<ValPos> rhs2(new StringValPos(4));																		\
	char* RHSVal2 = queryValue2;																					\
	rhs2->set((byte*)RHSVal2);																						\
	ds2->changeRHSBinding(rhs2.get());																				\
																													\
	DataSource* srcs[2] = { ds1.get(), ds2.get() };																	\
	int numCols[2] = { 1, 1 };																						\
																													\
																													\
	auto_ptr<BlockPrinter> bPrint(new BlockPrinter(srcs, numCols, 2, "Query4S.out"));								\
	BlockPrinter::MULTITHREAD_WAY = queryWay;																		\
	bPrint->wrapperOfPrintColumns(true, false);																		\
																													\
	aryQueryResult = bPrint->m_aryQueryResult;																		\
}
TEST_F(TestBlockPrinter, TestMethod_PrintColumns)
{
	bool bTestAll = true;
	const int TestCase_Index = 3;

	if (bTestAll || TestCase_Index == 1)
	{//Query1S
		std::vector<std::string> aryQueryResult;

		char* cpValue1 = "abcde00100";
		char* cpValue2 = "Cstore00100";

		char* file1 = "StringTest01.ID";
		char* file2 = "StringTest01.DESC";

		// Single Thread
		TestCase1_Query1S(file1, file2, 1, aryQueryResult, cpValue1, Predicate::OP_EQUAL, cpValue2, Predicate::OP_EQUAL);

		EXPECT_EQ(2, aryQueryResult.size());
		if (aryQueryResult.size() > 0)
		{
			EXPECT_EQ(cpValue1, aryQueryResult[0]);
			EXPECT_EQ(cpValue2, aryQueryResult[1]);
		}

		aryQueryResult.clear();
		// Multi-Thread

		TestCase1_Query1S(file1, file2, 2, aryQueryResult, cpValue1, Predicate::OP_LESS_THAN_OR_EQUAL, cpValue2, Predicate::OP_LESS_THAN_OR_EQUAL);

		EXPECT_EQ(100, aryQueryResult.size());
		bool bFindValue1 = false;
		bool bFindValue2 = false;
		for (unsigned int i = 0; i < aryQueryResult.size(); ++i)
		{
			if (aryQueryResult[i] == cpValue1)
			{
				bFindValue1 = true;
			}
			if (aryQueryResult[i] == cpValue2)
			{
				bFindValue2 = true;
			}
		}

		EXPECT_TRUE(bFindValue1) << "Find the value (" << cpValue1 << ") in column1";
		EXPECT_TRUE(bFindValue2) << "Find the value (" << cpValue2 << ") in column2";

		aryQueryResult.clear();
		// OpenMP
		TestCase1_Query1S(file1, file2, 3, aryQueryResult, cpValue1, Predicate::OP_EQUAL, cpValue2, Predicate::OP_EQUAL);

		EXPECT_EQ(2, aryQueryResult.size());
		if (aryQueryResult.size() > 0)
		{
			EXPECT_EQ(cpValue1, aryQueryResult[0]);
			EXPECT_EQ(cpValue2, aryQueryResult[1]);
		}
	}
	
	if (bTestAll || TestCase_Index == 2)
	{//Query2S
		std::vector<std::string> aryQueryResult;
		
		char* cpValue1 = "abc0000050";
		char* cpValue2 = "LRCS0000050";

		char* file1 = "StringTest02.ID";
		char* file2 = "StringTest02.DESC";

		// Single Thread
		TestCase1_Query2S(file1, file2, 1, aryQueryResult, cpValue1, Predicate::OP_EQUAL, cpValue2, Predicate::OP_EQUAL);

		EXPECT_EQ(2, aryQueryResult.size());
		if (aryQueryResult.size() > 0)
		{
			EXPECT_EQ(cpValue1, aryQueryResult[0]);
			EXPECT_EQ(cpValue2, aryQueryResult[1]);
		}

		aryQueryResult.clear();
		// Multi-Thread

		TestCase1_Query2S(file1, file2, 2, aryQueryResult, cpValue1, Predicate::OP_LESS_THAN_OR_EQUAL, cpValue2, Predicate::OP_LESS_THAN_OR_EQUAL);

		EXPECT_EQ(100, aryQueryResult.size());
		bool bFindValue1 = false;
		bool bFindValue2 = false;
		for (unsigned int i = 0; i < aryQueryResult.size(); ++i)
		{
			if (aryQueryResult[i] == cpValue1)
			{
				bFindValue1 = true;
			}
			if (aryQueryResult[i] == cpValue2)
			{
				bFindValue2 = true;
			}
		}

		EXPECT_TRUE(bFindValue1) << "Find the value (" << cpValue1 << ") in column1";
		EXPECT_TRUE(bFindValue2) << "Find the value (" << cpValue2 << ") in column2";

		aryQueryResult.clear();
		// OpenMP
		TestCase1_Query2S(file1, file2, 3, aryQueryResult, cpValue1, Predicate::OP_EQUAL, cpValue2, Predicate::OP_EQUAL);

		EXPECT_EQ(2, aryQueryResult.size());
		if (aryQueryResult.size() > 0)
		{
			EXPECT_EQ(cpValue1, aryQueryResult[0]);
			EXPECT_EQ(cpValue2, aryQueryResult[1]);
		}
	}


	if (bTestAll || TestCase_Index == 4)
	{//Query4S
		std::vector<std::string> aryQueryResult;

		char* cpValue1 = "abc0000050";
		char* cpValue2 = "2001";

		char* file1 = "StringTest04.ID";
		char* file2 = "StringTest04.CODE";
		// Single Thread
		TestCase1_Query4S(file1, file2, 1, aryQueryResult, cpValue1, Predicate::OP_EQUAL, cpValue2, Predicate::OP_EQUAL);

		EXPECT_EQ(2, aryQueryResult.size());
		if (aryQueryResult.size() > 0)
		{
			EXPECT_EQ(cpValue1, aryQueryResult[0]);
			EXPECT_EQ(cpValue2, aryQueryResult[1]);
		}

		aryQueryResult.clear();
		// Multi-Thread

		TestCase1_Query4S(file1, file2, 2, aryQueryResult, cpValue1, Predicate::OP_LESS_THAN_OR_EQUAL, cpValue2, Predicate::OP_LESS_THAN_OR_EQUAL);

		EXPECT_EQ(100, aryQueryResult.size());
		bool bFindValue1 = false;
		bool bFindValue2 = false;
		for (unsigned int i = 0; i < aryQueryResult.size(); ++i)
		{
			if (aryQueryResult[i] == cpValue1)
			{
				bFindValue1 = true;
			}
			if (aryQueryResult[i] == cpValue2)
			{
				bFindValue2 = true;
			}
		}

		EXPECT_TRUE(bFindValue1) << "Find the value (" << cpValue1 << ") in column1";
		EXPECT_TRUE(bFindValue2) << "Find the value (" << cpValue2 << ") in column2";

		aryQueryResult.clear();
		// OpenMP
		TestCase1_Query4S(file1, file2, 3, aryQueryResult, cpValue1, Predicate::OP_EQUAL, cpValue2, Predicate::OP_EQUAL);

		EXPECT_EQ(2, aryQueryResult.size());
		if (aryQueryResult.size() > 0)
		{
			EXPECT_EQ(cpValue1, aryQueryResult[0]);
			EXPECT_EQ(cpValue2, aryQueryResult[1]);
		}
	}
	
	if (bTestAll || TestCase_Index == 3)
	{//Query3S
		std::vector<std::string> aryQueryResult;

		char* cpValue1 = "abc0000050";
		char* cpValue2 = "8000";

		char* file1 = "StringTest03.ID";
		char* file2 = "StringTest03.CODE";
		// Single Thread
		TestCase1_Query3S(file1, file2, 1, aryQueryResult, cpValue1, Predicate::OP_EQUAL, cpValue2, Predicate::OP_EQUAL);

		EXPECT_EQ(2, aryQueryResult.size());
		if (aryQueryResult.size() > 0)
		{
			EXPECT_EQ(cpValue1, aryQueryResult[0]);
			EXPECT_EQ(cpValue2, aryQueryResult[1]);
		}

		aryQueryResult.clear();
		// Multi-Thread

		TestCase1_Query3S(file1, file2, 2, aryQueryResult, cpValue1, Predicate::OP_LESS_THAN_OR_EQUAL, cpValue2, Predicate::OP_LESS_THAN_OR_EQUAL);

		EXPECT_EQ(82, aryQueryResult.size());
		bool bFindValue1 = false;
		bool bFindValue2 = false;
		for (unsigned int i = 0; i < aryQueryResult.size(); ++i)
		{
			if (aryQueryResult[i] == cpValue1)
			{
				bFindValue1 = true;
			}
			if (aryQueryResult[i] == cpValue2)
			{
				bFindValue2 = true;
			}
		}

		EXPECT_TRUE(bFindValue1) << "Find the value (" << cpValue1 << ") in column1";
		EXPECT_TRUE(bFindValue2) << "Find the value (" << cpValue2 << ") in column2";

		aryQueryResult.clear();
		// OpenMP
		TestCase1_Query3S(file1, file2, 3, aryQueryResult, cpValue1, Predicate::OP_EQUAL, cpValue2, Predicate::OP_EQUAL);

		EXPECT_EQ(2, aryQueryResult.size());
		if (aryQueryResult.size() > 0)
		{
			EXPECT_EQ(cpValue1, aryQueryResult[0]);
			EXPECT_EQ(cpValue2, aryQueryResult[1]);
		}
	}

}


// Please write the test code above this block.
#endif

// Please DON'T write the code out of the "USE_GTEST" block.