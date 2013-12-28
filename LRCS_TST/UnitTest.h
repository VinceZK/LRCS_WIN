#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "stdafx.h"

class Globals {
public:
	bool build_tables;
	bool force_rebuild;
};


using namespace std;
class UnitTest
{
public:
	UnitTest();
	virtual ~UnitTest();
	virtual bool run(char* arg1, char* arg2) = 0;
protected:
	bool test(char* msg_, int retBool_, int val_, int exp_);
};

