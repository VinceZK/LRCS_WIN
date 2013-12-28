#pragma once
#include "UnitTest.h"
class ProjMakerTst :
	public UnitTest
{
public:
	ProjMakerTst();
	~ProjMakerTst();
	bool run(char* arg1, char* arg2);
};

