#pragma once

#include <omp.h>
#include "DataSource.h"
#include "MultiPosFilterBlock.h"

///
//  Provide some common functions for the multi-thread/parallel
//
class ParallelHelper
{
public:
	ParallelHelper();
	~ParallelHelper();

public:	// OpenMV(Parallel)
	// Get the optimal thread number for the parallel
	static inline int GetThreadNumber(int nLoopNum)
	{
		const int g_ncore = omp_get_num_procs();
		const int MIN_ITERATOR_NUM = 4;
		int max_tn = nLoopNum / MIN_ITERATOR_NUM;
		int tn = max_tn > g_ncore ? g_ncore : max_tn;
		if (tn < 1)
		{
			tn = 1;
		}
		return tn;
	}

public:	// Multi-Thread (_beginthreadex(...) api)
	///////////////////////////////////////////////////////////////////
	// These methods are for the multi-thread in BlockPrinter.cpp.
	struct tParams
	{
		DataSource* pDataSrc;
		MultiPosFilterBlock* pResult;
	};

	static unsigned __stdcall threadFunc_DataSrc(void* params)
	{
		tParams* pThis = (tParams*)params;

		pThis->pResult = pThis->pDataSrc->getPosOnPred();

		return 1;
	}
	//
	///////////////////////////////////////////////////////////////////
};

