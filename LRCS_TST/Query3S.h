#pragma once
#include "AM.h"
#include "SColumnExtracter.h"
#include "StringEncoder.h"
#include "PageWriter.h"
#include "Constants.h"
#include "Util.h"
#include "CodingException.h"
#include "StringDecoder.h"
#include "StringDataSource.h"
#include "DataSource.h"
#include "BlockPrinter.h"
#include "PagePlacer.h"
#include "UnitTest.h"
class Query3S :
	public UnitTest
{
public:
	Query3S();
	virtual ~Query3S();
	bool run(char* arg1, char* arg2);
};

