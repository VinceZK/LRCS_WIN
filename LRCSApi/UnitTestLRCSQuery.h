#pragma once

#ifdef USE_GTEST
#include <gtest/gtest.h>
#include "LRCSQuery.h"

class TestLRCSQuery : public testing::Test
{
public:
	TestLRCSQuery();
	virtual ~TestLRCSQuery();

protected:
	void SetUp(){
		EXPECT_TRUE(query.Init());
	}
	void TearDown() {
		query.Destroy();

		//EXPECT_TRUE(m_dwEndTime - m_dwBeginTime < 100) << "The performance is too low.";
	}

	LRCSQuery query;
};

#endif