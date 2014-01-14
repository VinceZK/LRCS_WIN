#include "stdafx.h"
#include "..\LRCSBasis\BlockPrinter.h"

// Please DON'T write the code out of the "USE_GTEST" block.

#ifdef USE_GTEST
// Please write the test code under this block.

#include "..\LRCSBasis\LZDataSource.h"
#include "..\LRCSBasis\RLEDataSource.h"

class TestBlockPrinter : public testing::Test
{
protected:
	void SetUp(){
		m_dwBeginTime = GetTickCount();
	}
	void TearDown() {
		m_dwEndTime = GetTickCount();

		EXPECT_TRUE(m_dwEndTime - m_dwBeginTime < 100) << "The performance is too low.";
	}

private:
	DWORD m_dwBeginTime;
	DWORD m_dwEndTime;
};

TEST_F(TestBlockPrinter, TestMethod_PrintColumns)
{
	ROSAM* am1 = new ROSAM("StringTest04.ID", 2, sizeof(int), 10, ValPos::INTTYPE, ValPos::STRINGTYPE);
	ROSAM* am2 = new ROSAM("StringTest04.CODE", 2, sizeof(int), 4, ValPos::INTTYPE, ValPos::STRINGTYPE);
	Predicate* pred1 = new Predicate(Predicate::OP_GREATER_THAN_OR_EQUAL);
	Predicate* pred2 = new Predicate(Predicate::OP_GREATER_THAN_OR_EQUAL);
	LZDataSource* ds1 = new LZDataSource(am1, true, true, (new StringDecoder(true)));
	RLEDataSource* ds2 = new RLEDataSource(am2, false, true, (new StringDecoder(true)));

	ValPos* rhs1 = new StringValPos(10);
	char* RHSVal1 = "abc0000015";
	rhs1->set((byte*)RHSVal1);
	ValPos* rhs2 = new StringValPos(4);
	char* RHSVal2 = "2001";
	rhs2->set((byte*)RHSVal2);

	ds1->setPredicate(pred1);
	ds1->changeRHSBinding(rhs1);
	ds2->setPredicate(pred2);
	ds2->changeRHSBinding(rhs2);

	DataSource* srcs[2] = { ds1, ds2 };
	int numCols[2] = { 1, 1 };

	BlockPrinter* bPrint = new BlockPrinter(srcs, numCols, 2, "Query4S.out");

	bPrint->MULTITHREAD_WAY = 1;	//Single Thread
	bPrint->wrapperOfPrintColumns(true, false);

	
	//EXPECT_EQ();
}

// Please write the test code above this block.
#endif

// Please DON'T write the code out of the "USE_GTEST" block.